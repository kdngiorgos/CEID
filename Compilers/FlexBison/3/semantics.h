#ifndef SEMANTICS_H
#define SEMANTICS_H

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_VARCHAR,
    TYPE_UNKNOWN
} ColType;

#define MAX_COLS    64
#define MAX_TABLES  128
#define MAX_ALIASES 64

typedef struct {
    char name[64];
    ColType type;
    int varchar_size;   /* mono gia VARCHAR */
} Column;

typedef struct {
    char name[64];
    Column cols[MAX_COLS];
    int col_count;
} Table;

typedef struct {
    char alias[64];
    char table[64];   /* to pragmatiko onoma tis tablas */
} AliasEntry;

/* dimiourgia tablas sto symbol table */
void sem_create_table(const char *name, Column *cols, int col_count);

/* elegxei an uparxei h tabla */
void sem_from_check(const char *name);

/* elegxei an uparxei to col stin tabla */
void sem_column_exists(const char *table, const char *col);

/* type check col vs literal */
void sem_type_check(const char *table, const char *col, ColType lit_type);

/* type check gia IN list */
void sem_type_check_in(const char *table, const char *col,
                       ColType *lit_types, int lit_count);

/* katagrafh alias -> tabla */
void sem_register_alias(const char *alias, const char *table);
const char *sem_resolve_alias(const char *name);

/* orizoume tis active tables tou trexontos SELECT */
void sem_set_query_tables(const char **names, int count);

/* elegxos col mesa sto trexon query context (NULL = bare col) */
void sem_column_in_query(const char *table_or_null, const char *col);

/* reset aliases/active tables metaksy SELECT statements */
void sem_reset_aliases(void);

#endif /* SEMANTICS_H */
