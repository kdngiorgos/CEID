#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantics.h"

/* to global symbol table - gemizei me kathe CREATE TABLE */
static Table symbol_table[MAX_TABLES];
static int   table_count = 0;

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
    if (!find_table(name)) {
        fprintf(stderr, "Semantic error: unknown table '%s'.\n", name);
    }
}

void sem_column_exists(const char *table, const char *col) {
    Table *t = find_table(table);
    if (!t) {
        fprintf(stderr, "Semantic error: unknown table '%s'.\n", table);
        return;
    }
    if (!find_column(t, col)) {
        fprintf(stderr,
            "Semantic error: column '%s' not found in table '%s'.\n",
            col, table);
    }
}

void sem_type_check(const char *table, const char *col, ColType lit_type) {
    Table *t = find_table(table);
    if (!t) {
        fprintf(stderr, "Semantic error: unknown table '%s'.\n", table);
        return;
    }
    Column *c = find_column(t, col);
    if (!c) {
        fprintf(stderr,
            "Semantic error: column '%s' not found in table '%s'.\n",
            col, table);
        return;
    }

    /* FLOAT dektetai kai INT literal (implicit cast), alla oxi to antistrofo */
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
            table, col, coltype_str(c->type), coltype_str(lit_type));
    }
}

void sem_type_check_in(const char *table, const char *col,
                       ColType *lit_types, int lit_count) {
    Table *t = find_table(table);
    if (!t) {
        fprintf(stderr, "Semantic error: unknown table '%s'.\n", table);
        return;
    }
    Column *c = find_column(t, col);
    if (!c) {
        fprintf(stderr,
            "Semantic error: column '%s' not found in table '%s'.\n",
            col, table);
        return;
    }

    /* idia logiki me type_check, apla gia kathe stoixeio tis listas */
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
                table, col, coltype_str(c->type), coltype_str(lit_types[i]));
        }
    }
}
