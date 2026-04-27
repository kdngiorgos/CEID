#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantics.h"

/* global symbol table - gemizei progressiva me ta CREATE TABLE */
static Table symbol_table[MAX_TABLES];
static int   table_count = 0;

/* alias map kai active tables - reset se kathe neo SELECT */
static AliasEntry  alias_map[MAX_ALIASES];
static int         alias_count = 0;

static const char *active_tables[MAX_ALIASES];
static int         active_table_count = 0;

static Table *find_table(const char *name) {
    for (int i = 0; i < table_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0)
            return &symbol_table[i];
    }
    return NULL;
}

static Column *find_column(Table *t, const char *col) {
    for (int i = 0; i < t->col_count; i++) {
        if (strcmp(t->cols[i].name, col) == 0)
            return &t->cols[i];
    }
    return NULL;
}

static const char *coltype_str(ColType t) {
    switch (t) {
        case TYPE_INT:     return "INT";
        case TYPE_FLOAT:   return "FLOAT";
        case TYPE_VARCHAR: return "VARCHAR";
        default:           return "UNKNOWN";
    }
}

void sem_create_table(const char *name, Column *cols, int col_count) {
    if (find_table(name)) {
        fprintf(stderr, "Semantic error: table '%s' already defined.\n", name);
        return;
    }

    /* elegxos gia duplicate cols mesa stin idia tabla */
    for (int i = 0; i < col_count; i++) {
        for (int j = i + 1; j < col_count; j++) {
            if (strcmp(cols[i].name, cols[j].name) == 0) {
                fprintf(stderr,
                    "Semantic error: duplicate column '%s' in table '%s'.\n",
                    cols[i].name, name);
                return;
            }
        }
    }

    if (table_count >= MAX_TABLES) {
        fprintf(stderr, "Semantic error: too many tables (max %d).\n", MAX_TABLES);
        return;
    }

    Table *t = &symbol_table[table_count++];
    strncpy(t->name, name, sizeof(t->name) - 1);
    t->col_count = col_count;
    for (int i = 0; i < col_count; i++)
        t->cols[i] = cols[i];
}

void sem_from_check(const char *name) {
    const char *real = sem_resolve_alias(name);
    if (!find_table(real)) {
        fprintf(stderr, "Semantic error: unknown table '%s'.\n", real);
    }
}

void sem_column_exists(const char *table, const char *col) {
    const char *real = sem_resolve_alias(table);
    Table *t = find_table(real);
    if (!t) {
        fprintf(stderr, "Semantic error: unknown table '%s'.\n", real);
        return;
    }
    if (!find_column(t, col)) {
        fprintf(stderr,
            "Semantic error: column '%s' not found in table '%s'.\n",
            col, real);
    }
}

void sem_type_check(const char *table, const char *col, ColType lit_type) {
    const char *real = sem_resolve_alias(table);
    Table *t = find_table(real);
    if (!t) {
        fprintf(stderr, "Semantic error: unknown table '%s'.\n", real);
        return;
    }
    Column *c = find_column(t, col);
    if (!c) {
        fprintf(stderr,
            "Semantic error: column '%s' not found in table '%s'.\n",
            col, real);
        return;
    }

    /* FLOAT column mporei na dextei INT literal (widening), to antistrofo oxi */
    int ok = 0;
    if (c->type == TYPE_INT && lit_type == TYPE_INT)
        ok = 1;
    else if (c->type == TYPE_FLOAT && (lit_type == TYPE_INT || lit_type == TYPE_FLOAT))
        ok = 1;
    else if (c->type == TYPE_VARCHAR && lit_type == TYPE_VARCHAR)
        ok = 1;

    if (!ok) {
        fprintf(stderr,
            "Semantic error: type mismatch — column '%s.%s' is %s, "
            "but literal is %s.\n",
            real, col, coltype_str(c->type), coltype_str(lit_type));
    }
}

void sem_type_check_in(const char *table, const char *col,
                       ColType *lit_types, int lit_count) {
    const char *real = sem_resolve_alias(table);
    Table *t = find_table(real);
    if (!t) {
        fprintf(stderr, "Semantic error: unknown table '%s'.\n", real);
        return;
    }
    Column *c = find_column(t, col);
    if (!c) {
        fprintf(stderr,
            "Semantic error: column '%s' not found in table '%s'.\n",
            col, real);
        return;
    }

    /* idia logiki me type_check, apla looping gia kathe stoixeio */
    for (int i = 0; i < lit_count; i++) {
        int ok = 0;
        if (c->type == TYPE_INT && lit_types[i] == TYPE_INT)
            ok = 1;
        else if (c->type == TYPE_FLOAT &&
                 (lit_types[i] == TYPE_INT || lit_types[i] == TYPE_FLOAT))
            ok = 1;
        else if (c->type == TYPE_VARCHAR && lit_types[i] == TYPE_VARCHAR)
            ok = 1;

        if (!ok) {
            fprintf(stderr,
                "Semantic error: type mismatch in IN list — column '%s.%s' "
                "is %s, but literal is %s.\n",
                real, col, coltype_str(c->type), coltype_str(lit_types[i]));
        }
    }
}

void sem_register_alias(const char *alias, const char *table) {
    /* den epitrepoume to idio alias dis sto idio query */
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(alias_map[i].alias, alias) == 0) {
            fprintf(stderr,
                "Semantic error: alias '%s' already defined in this query.\n",
                alias);
            return;
        }
    }
    if (alias_count >= MAX_ALIASES) {
        fprintf(stderr, "Semantic error: too many aliases.\n");
        return;
    }
    AliasEntry *e = &alias_map[alias_count++];
    strncpy(e->alias, alias, sizeof(e->alias) - 1);
    strncpy(e->table, table, sizeof(e->table) - 1);
}

const char *sem_resolve_alias(const char *name) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(alias_map[i].alias, name) == 0)
            return alias_map[i].table;
    }
    return name; /* den einai alias, epistrefoume ws exei */
}

void sem_set_query_tables(const char **names, int count) {
    active_table_count = 0;
    for (int i = 0; i < count && i < MAX_ALIASES; i++)
        active_tables[active_table_count++] = names[i];
}

void sem_column_in_query(const char *table_or_null, const char *col) {
    if (table_or_null) {
        /* qualified ref (tabla.col) - apeftheias elegxos */
        sem_column_exists(table_or_null, col);
        return;
    }

    /* bare col: psaxnoume se oles tis active tables */
    if (active_table_count == 0) return;

    int found = 0;
    for (int i = 0; i < active_table_count; i++) {
        const char *real = sem_resolve_alias(active_tables[i]);
        Table *t = find_table(real);
        if (t && find_column(t, col)) {
            found++;
        }
    }
    if (found == 0) {
        fprintf(stderr,
            "Semantic error: column '%s' not found in any active table.\n",
            col);
    } else if (found > 1) {
        /* ambiguous - uparxei se perissoteres tables, prepei na to kaneis qualify */
        fprintf(stderr,
            "Semantic warning: column '%s' is ambiguous (found in %d tables).\n",
            col, found);
    }
}

void sem_reset_aliases(void) {
    alias_count        = 0;
    active_table_count = 0;
}
