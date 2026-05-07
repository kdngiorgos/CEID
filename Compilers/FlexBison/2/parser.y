/*
KANDILAS GEORGIOS AM:1115510
KARPETAS APOSTOLOS AM:1115507
*/

/*Q2, syntaktiki + semantiki analysi */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantics.h"

#define MAX_LINES 4096
#define MAX_LINE_LEN 1024

static char *lines[MAX_LINES];
static int   line_count = 0;

int  yylex(void);
void yyerror(const char *s);
extern int yylineno;
extern FILE *yyin;

/* Buffer */
/* buffer gia ta cols tou CREATE TABLE, gemizei me kathe col_def */
#define MAX_COLS_PARSE 64
static Column create_cols[MAX_COLS_PARSE];
static int    create_col_count = 0;

/* h tabla pou einai active sto trexon SELECT */
static char *current_query_table = NULL;

/* ta cols tou SELECT ta apothikevw edw kai ta elegxo meta to FROM
   giati o parser ta vlepei prin mathi tin tabla */
#define MAX_SELECT_COLS 64
static char *select_cols[MAX_SELECT_COLS];
static int   select_col_count = 0;

/* buffer typwn gia IN/NOT IN list */
#define MAX_IN_LITERALS 256
static ColType in_literal_types[MAX_IN_LITERALS];
static int     in_literal_count = 0;
%}

%union {
    char  *str;
    int    ival;
    float  fval;
    int    type_tag;
}

%token TOK_SELECT TOK_FROM TOK_WHERE TOK_LIMIT TOK_GROUP TOK_ORDER TOK_BY
%token TOK_IN TOK_AND TOK_OR TOK_NOT
%token TOK_CREATE TOK_TABLE
%token TOK_INT_TYPE TOK_FLOAT_TYPE TOK_VARCHAR
%token TOK_EQ TOK_NEQ TOK_GT TOK_LT TOK_GTE TOK_LTE
%token TOK_STAR

%token <str>  TOK_IDENT TOK_STR_LIT
%token <ival> TOK_INT_LIT
%token <fval> TOK_FLOAT_LIT

/* protereotita: OR < AND < NOT */
%left  TOK_OR
%left  TOK_AND
%right TOK_NOT

%type <str>      col_ref
%type <ival>     comp_op
%type <type_tag> literal type_spec

%%

program
    : stmt_list
    ;

/* eite adeio eite exei statement */
stmt_list
    : /* empty */
    | stmt_list stmt
    ;

/* 2 OPS, CREATE kai SELECT */
stmt
    : create_stmt
    | select_stmt
    ;

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

select_stmt
    : TOK_SELECT
        {
            if (current_query_table) free(current_query_table);
            current_query_table = NULL;
            select_col_count = 0;
        }
      col_expr_list TOK_FROM TOK_IDENT
        {
            sem_from_check($5);
            current_query_table = strdup($5);
            free($5);
        }
      opt_where opt_group opt_order opt_limit ';'
        {
            /* elegxos ton SELECT cols - deferred giati erthane prin to FROM */
            for (int i = 0; i < select_col_count; i++) {
                char *ref = select_cols[i];
                sem_column_exists(current_query_table, ref);
                free(ref);
            }
            select_col_count = 0;
            if (current_query_table) {
                free(current_query_table);
                current_query_table = NULL;
            }
        }
    ;

col_expr_list
    : TOK_STAR
    | select_col_ref_list
    ;

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
            sem_column_exists(current_query_table, $1);
            free($1);
        }
    | col_ref_list ',' col_ref
        {
            sem_column_exists(current_query_table, $3);
            free($3);
        }
    ;

col_ref
    : TOK_IDENT
        {
            $$ = strdup($1);
            free($1);
        }
    ;

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
            sem_type_check(current_query_table, $1, (ColType)$3);
            free($1);
        }
    | col_ref TOK_IN '('
        { in_literal_count = 0; }
      in_literal_list ')'
        {
            sem_type_check_in(current_query_table, $1, in_literal_types, in_literal_count);
            free($1);
        }
    | col_ref TOK_NOT TOK_IN '('
        { in_literal_count = 0; }
      in_literal_list ')'
        {
            sem_type_check_in(current_query_table, $1, in_literal_types, in_literal_count);
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

void yyerror(const char *s) {
    for (int i = 0; i < line_count && i < yylineno; i++) {
        printf("%s\n", lines[i]);
    }
    printf("Syntax error on line %d: %s\n", yylineno, s);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input.sql>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return 1;
    }

    char buf[MAX_LINE_LEN];
    while (fgets(buf, sizeof(buf), f) && line_count < MAX_LINES) {
        int len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
        lines[line_count++] = strdup(buf);
    }

    rewind(f);
    yyin = f;

    int result = yyparse();
    fclose(f);

    if (result == 0) {
        for (int i = 0; i < line_count; i++)
            printf("%s\n", lines[i]);
        printf("Syntactic analysis successful.\n");
    }

    for (int i = 0; i < line_count; i++)
        free(lines[i]);

    return result;
}
