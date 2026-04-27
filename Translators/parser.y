%{
/*
 * parser.y — mySQLq Bison parser
 * Handles: CREATE TABLE, SELECT with JOIN, WHERE, GROUP BY, ORDER BY, LIMIT.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantics.h"

/* ------------------------------------------------------------------ */
/*  Line storage for error reporting                                    */
/* ------------------------------------------------------------------ */
#define MAX_LINES 4096
#define MAX_LINE_LEN 1024

static char *lines[MAX_LINES];
static int   line_count = 0;

/* Forward declarations */
int  yylex(void);
void yyerror(const char *s);
extern int yylineno;
extern FILE *yyin;

/* ------------------------------------------------------------------ */
/*  Column list builder (used by CREATE TABLE)                          */
/* ------------------------------------------------------------------ */
#define MAX_COLS_PARSE 64
static Column create_cols[MAX_COLS_PARSE];
static int    create_col_count = 0;

/* ------------------------------------------------------------------ */
/*  Active table list builder (used by SELECT FROM + JOIN)             */
/* ------------------------------------------------------------------ */
#define MAX_QUERY_TABLES 64
static const char *query_tables[MAX_QUERY_TABLES];
static int         query_table_count = 0;

static void add_query_table(const char *name) {
    if (query_table_count < MAX_QUERY_TABLES)
        query_tables[query_table_count++] = name;
}

/* ------------------------------------------------------------------ */
/*  Deferred SELECT column list (validated after FROM/JOIN)            */
/* ------------------------------------------------------------------ */
#define MAX_SELECT_COLS 64
static char *select_cols[MAX_SELECT_COLS];
static int   select_col_count = 0;

/* ------------------------------------------------------------------ */
/*  IN literal type buffer (for Q2e.iii type checking)                 */
/* ------------------------------------------------------------------ */
#define MAX_IN_LITERALS 256
static ColType in_literal_types[MAX_IN_LITERALS];
static int     in_literal_count = 0;
%}

/* ------------------------------------------------------------------ */
/*  Value types                                                         */
/* ------------------------------------------------------------------ */
%union {
    char  *str;
    int    ival;
    float  fval;
    int    type_tag;   /* ColType value */
}

/* ------------------------------------------------------------------ */
/*  Tokens                                                              */
/* ------------------------------------------------------------------ */
%token TOK_SELECT TOK_FROM TOK_WHERE TOK_LIMIT TOK_GROUP TOK_ORDER TOK_BY
%token TOK_IN TOK_AND TOK_OR TOK_NOT
%token TOK_CREATE TOK_TABLE TOK_JOIN TOK_ON TOK_AS
%token TOK_INT_TYPE TOK_FLOAT_TYPE TOK_VARCHAR
%token TOK_EQ TOK_NEQ TOK_GT TOK_LT TOK_GTE TOK_LTE
%token TOK_STAR

%token <str>  TOK_IDENT TOK_STR_LIT
%token <ival> TOK_INT_LIT
%token <fval> TOK_FLOAT_LIT

/* ------------------------------------------------------------------ */
/*  Operator precedence (lowest → highest)                             */
/* ------------------------------------------------------------------ */
%left  TOK_OR
%left  TOK_AND
%right TOK_NOT 

/* Non-terminal types */
%type <str>      col_ref
%type <ival>     comp_op
%type <type_tag> literal type_spec

%%

/* ================================================================== */
program
    : stmt_list
    ;

stmt_list
    : /* empty */
    | stmt_list stmt
    ;

stmt
    : create_stmt
    | select_stmt
    ;

/* ================================================================== */
/*  CREATE TABLE                                                        */
/* ================================================================== */
create_stmt
    : TOK_CREATE TOK_TABLE TOK_IDENT '(' col_def_list ')' ';'
        {
            sem_create_table($3, create_cols, create_col_count);
            create_col_count = 0;
            free($3);
        }
    ;

col_def_list
    : col_def
    | col_def_list ',' col_def
    ;

col_def
    : TOK_IDENT type_spec
        {
            if (create_col_count < MAX_COLS_PARSE) {
                Column *c = &create_cols[create_col_count++];
                strncpy(c->name, $1, sizeof(c->name) - 1);
                c->type = (ColType)$2;
                c->varchar_size = 0;
            }
            free($1);
        }
    | TOK_IDENT TOK_VARCHAR '(' TOK_INT_LIT ')'
        {
            if ($4 <= 0) {
                fprintf(stderr,
                    "Semantic error: varchar size must be a positive integer.\n");
            }
            if (create_col_count < MAX_COLS_PARSE) {
                Column *c = &create_cols[create_col_count++];
                strncpy(c->name, $1, sizeof(c->name) - 1);
                c->type = TYPE_VARCHAR;
                c->varchar_size = $4;
            }
            free($1);
        }
    ;

type_spec
    : TOK_INT_TYPE   { $$ = TYPE_INT; }
    | TOK_FLOAT_TYPE { $$ = TYPE_FLOAT; }
    ;

/* ================================================================== */
/*  SELECT                                                              */
/* ================================================================== */
select_stmt
    : TOK_SELECT
        { sem_reset_aliases(); query_table_count = 0; select_col_count = 0; }
      col_expr_list TOK_FROM table_ref join_list
        {
            /* Set active tables now so WHERE/GROUP BY/ORDER BY can validate */
            sem_set_query_tables(query_tables, query_table_count);
        }
      opt_where opt_group opt_order opt_limit ';'
        {
            /* Validate SELECT columns (deferred — parsed before FROM) */
            for (int i = 0; i < select_col_count; i++) {
                char *ref = select_cols[i];
                char *dot = strchr(ref, '.');
                if (dot) {
                    *dot = '\0';
                    sem_column_in_query(ref, dot + 1);
                } else {
                    sem_column_in_query(NULL, ref);
                }
                free(ref);
            }
            select_col_count = 0;
        }
    ;

/* ---- FROM table reference ---- */
table_ref
    : TOK_IDENT
        {
            sem_from_check($1);
            add_query_table(strdup($1));
            free($1);
        }
    | TOK_IDENT TOK_AS TOK_IDENT
        {
            sem_from_check($1);
            sem_register_alias($3, $1);
            add_query_table(strdup($3));
            free($1); free($3);
        }
    ;

/* ---- JOIN list ---- */
join_list
    : /* empty */
    | join_list TOK_JOIN table_ref TOK_ON col_ref TOK_EQ col_ref
        {
            /* Check both sides of the ON condition */
            char *dot;
            dot = strchr($5, '.');
            if (dot) { *dot = '\0'; sem_column_exists($5, dot + 1); *dot = '.'; }
            dot = strchr($7, '.');
            if (dot) { *dot = '\0'; sem_column_exists($7, dot + 1); *dot = '.'; }
            free($5); free($7);
        }
    ;

/* ---- Column expression list (SELECT clause) ---- */
col_expr_list
    : TOK_STAR
    | select_col_ref_list
    ;

/* Stores refs for deferred validation after FROM/JOIN are known */
select_col_ref_list
    : col_ref
        {
            if (select_col_count < MAX_SELECT_COLS)
                select_cols[select_col_count++] = $1;
            else
                free($1);
        }
    | select_col_ref_list ',' col_ref
        {
            if (select_col_count < MAX_SELECT_COLS)
                select_cols[select_col_count++] = $3;
            else
                free($3);
        }
    ;

col_ref_list
    : col_ref
        {
            char *dot = strchr($1, '.');
            if (dot) {
                *dot = '\0';
                sem_column_in_query($1, dot + 1);
            } else {
                sem_column_in_query(NULL, $1);
            }
            free($1);
        }
    | col_ref_list ',' col_ref
        {
            char *dot = strchr($3, '.');
            if (dot) {
                *dot = '\0';
                sem_column_in_query($3, dot + 1);
            } else {
                sem_column_in_query(NULL, $3);
            }
            free($3);
        }
    ;

/* col_ref returns a freshly-allocated "table.col" or "col" string */
col_ref
    : TOK_IDENT
        {
            $$ = strdup($1);
            /* Bare column: resolved after FROM/JOIN are processed */
            free($1);
        }
    | TOK_IDENT '.' TOK_IDENT
        {
            int len = strlen($1) + 1 + strlen($3) + 1;
            $$ = malloc(len);
            snprintf($$, len, "%s.%s", $1, $3);
            free($1); free($3);
        }
    ;

/* ================================================================== */
/*  WHERE clause                                                        */
/* ================================================================== */
opt_where
    : /* empty */
    | TOK_WHERE condition
    ;

condition
    : condition TOK_OR  condition
    | condition TOK_AND condition
    | TOK_NOT condition
    | '(' condition ')'
    | col_ref comp_op literal
        {
            /* type-check: split "table.col" or bare col */
            char *dot = strchr($1, '.');
            if (dot) {
                *dot = '\0';
                sem_type_check($1, dot + 1, (ColType)$3);
            } else {
                /* bare col: check against all active tables */
                for (int i = 0; i < query_table_count; i++) {
                    const char *real = sem_resolve_alias(query_tables[i]);
                    sem_type_check(real, $1, (ColType)$3);
                    break; /* check first match only to avoid duplicate errors */
                }
            }
            free($1);
        }
    | col_ref TOK_IN '('
        { in_literal_count = 0; }
      in_literal_list ')'
        {
            char *dot = strchr($1, '.');
            if (dot) {
                *dot = '\0';
                sem_type_check_in($1, dot + 1, in_literal_types, in_literal_count);
            } else {
                for (int i = 0; i < query_table_count; i++) {
                    const char *real = sem_resolve_alias(query_tables[i]);
                    sem_type_check_in(real, $1, in_literal_types, in_literal_count);
                    break;
                }
            }
            free($1);
        }
    | col_ref TOK_NOT TOK_IN '('
        { in_literal_count = 0; }
      in_literal_list ')'
        {
            char *dot = strchr($1, '.');
            if (dot) {
                *dot = '\0';
                sem_type_check_in($1, dot + 1, in_literal_types, in_literal_count);
            } else {
                for (int i = 0; i < query_table_count; i++) {
                    const char *real = sem_resolve_alias(query_tables[i]);
                    sem_type_check_in(real, $1, in_literal_types, in_literal_count);
                    break;
                }
            }
            free($1);
        }
    ;

comp_op
    : TOK_EQ  { $$ = 0; }
    | TOK_NEQ { $$ = 1; }
    | TOK_GT  { $$ = 2; }
    | TOK_LT  { $$ = 3; }
    | TOK_GTE { $$ = 4; }
    | TOK_LTE { $$ = 5; }
    ;

literal
    : TOK_INT_LIT   { $$ = TYPE_INT; }
    | TOK_FLOAT_LIT { $$ = TYPE_FLOAT; }
    | TOK_STR_LIT   { $$ = TYPE_VARCHAR; free($1); }
    ;

in_literal_list
    : literal
        {
            if (in_literal_count < MAX_IN_LITERALS)
                in_literal_types[in_literal_count++] = (ColType)$1;
        }
    | in_literal_list ',' literal
        {
            if (in_literal_count < MAX_IN_LITERALS)
                in_literal_types[in_literal_count++] = (ColType)$3;
        }
    ;

/* ================================================================== */
/*  GROUP BY / ORDER BY / LIMIT                                         */
/* ================================================================== */
opt_group
    : /* empty */
    | TOK_GROUP TOK_BY col_ref_list
    ;

opt_order
    : /* empty */
    | TOK_ORDER TOK_BY col_ref_list
    ;

opt_limit
    : /* empty */
    | TOK_LIMIT TOK_INT_LIT
    ;

%%

/* ================================================================== */
/*  yyerror                                                             */
/* ================================================================== */
void yyerror(const char *s) {
    /* Print source lines seen so far */
    for (int i = 0; i < line_count && i < yylineno; i++) {
        printf("%s\n", lines[i]);
    }
    printf("Syntax error on line %d: %s\n", yylineno, s);
}

/* ================================================================== */
/*  main                                                                */
/* ================================================================== */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input.sql>\n", argv[0]);
        return 1;
    }

    /* Read all lines for error reporting */
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return 1;
    }

    char buf[MAX_LINE_LEN];
    while (fgets(buf, sizeof(buf), f) && line_count < MAX_LINES) {
        /* Strip trailing newline for cleaner output */
        int len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
        lines[line_count++] = strdup(buf);
    }

    /* Rewind and set yyin */
    rewind(f);
    yyin = f;

    int result = yyparse();
    fclose(f);

    if (result == 0) {
        /* Print all source lines */
        for (int i = 0; i < line_count; i++)
            printf("%s\n", lines[i]);
        printf("Syntactic analysis successful.\n");
    }

    /* Free line storage */
    for (int i = 0; i < line_count; i++)
        free(lines[i]);

    return result;
}
