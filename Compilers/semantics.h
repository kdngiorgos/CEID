#ifndef SEMANTICS_H
#define SEMANTICS_H

/* ------------------------------------------------------------------ */
/*  Column type codes                                                   */
/* ------------------------------------------------------------------ */
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_VARCHAR,
    TYPE_UNKNOWN
} ColType;

/* ------------------------------------------------------------------ */
/*  Data structures                                                     */
/* ------------------------------------------------------------------ */

#define MAX_COLS    64
#define MAX_TABLES  128
#define MAX_ALIASES 64

typedef struct {
    char name[64];
    ColType type;
    int varchar_size;   /* only meaningful when type == TYPE_VARCHAR */
} Column;

typedef struct {
    char name[64];
    Column cols[MAX_COLS];
    int col_count;
} Table;

typedef struct {
    char alias[64];
    char table[64];
} AliasEntry;

/* ------------------------------------------------------------------ */
/*  Public API                                                          */
/* ------------------------------------------------------------------ */

/* Q2 — CREATE TABLE */
void sem_create_table(const char *name, Column *cols, int col_count);

/* Q2 — FROM / JOIN: verify table exists in symbol table */
void sem_from_check(const char *name);

/* Q2 — check column exists in given table */
void sem_column_exists(const char *table, const char *col);

/* Q2 — type check: literal type must match column type */
void sem_type_check(const char *table, const char *col, ColType lit_type);

/* Q2e.iii — type check for IN/NOT IN: each literal must match column type */
void sem_type_check_in(const char *table, const char *col,
                       ColType *lit_types, int lit_count);

/* Q3 — alias management */
void sem_register_alias(const char *alias, const char *table);
const char *sem_resolve_alias(const char *name);

/* Q3 — per-SELECT active table list (for bare column references) */
void sem_set_query_tables(const char **names, int count);

/* Q3 — check bare or qualified column in current query context */
void sem_column_in_query(const char *table_or_null, const char *col);

/* Reset per-query state between SELECT statements */
void sem_reset_aliases(void);

#endif /* SEMANTICS_H */
