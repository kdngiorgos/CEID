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

/* dimiourgia pinaka - pairnei onoma kai lista cols */
void sem_create_table(const char *name, Column *cols, int col_count);

/* elegxei an h tabla uparxei sto symbol table */
void sem_from_check(const char *name);

/* elegxei an to col uparxei stin tabla */
void sem_column_exists(const char *table, const char *col);

/* elegxei symvatotita typou col me literal */
void sem_type_check(const char *table, const char *col, ColType lit_type);

/* idio gia IN list - elegxei kathe stoixeio */
void sem_type_check_in(const char *table, const char *col,
                       ColType *lit_types, int lit_count);

#endif /* SEMANTICS_H */
