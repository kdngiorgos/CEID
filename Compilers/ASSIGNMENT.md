# mySQLq Compiler — Team Assignment

**Course:** Principles of Programming Languages & Compilers (Spring 2026, University of Patras)
**Team:** Giorgos, Thanos, Nasos
**Deadline:** May 31, 2026 (eclass submission)

---

## Project Overview

Build a **lexer + parser + semantic checker** for a SQL-like pseudo-language called **mySQLq**, using **Flex** (lexer), **Bison** (parser), and **C/C++** (semantic analysis).

### Grading Breakdown

| Question | Weight | Description |
|----------|--------|-------------|
| Q1 | 60% | BNF grammar + Flex lexer + Bison parser (syntax analysis) |
| Q2 | 15% | Semantic checks via C/C++ (symbol table, type checks) |
| Q3 | 25% | Extensions: JOIN support + table aliases (AS) |

### Component Architecture

```
[Input .sql file]
       ↓
  [Flex Lexer]  ──tokens──→  [Bison Parser]  ──actions──→  [Semantic Checker]
    (lexer.l)                  (parser.y)                   (semantics.c/.h)
    GIORGOS                     THANOS                         NASOS
```

All three components live in the same project. Semantic checks are triggered from Bison action blocks and implemented in separate C helper files.

---

## Work Breakdown

### Giorgos — Lexer (Flex) & BNF Terminal Side

**Core file:** `lexer.l`

#### Tasks

**Q1a (co-lead with Thanos):** Draft the BNF grammar — Giorgos owns the *terminal* (token-level) side: what the tokens look like.

**Q1b — Flex lexer:** Implement `lexer.l` recognizing:

| Category | Pattern |
|----------|---------|
| Keywords (case-insensitive) | `select from where limit group order by in and or not create table int float varchar join on as` (18 keywords) |
| Identifiers (case-sensitive) | `[a-zA-Z_][a-zA-Z0-9_]*` |
| Integer literals | Optional leading `-`, one or more digits |
| Float literals | Optional leading `-`, digits, `.`, digits |
| String literals | Single-quoted: `'...'` |
| Operators | `=  !=  >  <  >=  <=` |
| Special chars | `. , ; ( ) *` |
| Single-line comments | `--` to end of line → skip |
| Multi-line comments | `/* ... */` → skip |
| Whitespace/newlines | Skip, but **increment `yylineno`** on newlines |

**Q3 keywords:** `JOIN`, `ON`, `AS` must be in the lexer from the very beginning (even before Q3 work starts), so Thanos can reference them.

#### Interfaces to Thanos

- Define all token names in `tokens.h` (or let Bison generate them via `%token`). Agree on names like:
  - `TOK_SELECT`, `TOK_FROM`, `TOK_WHERE`, `TOK_LIMIT`, `TOK_GROUP`, `TOK_ORDER`, `TOK_BY`
  - `TOK_IN`, `TOK_AND`, `TOK_OR`, `TOK_NOT`
  - `TOK_CREATE`, `TOK_TABLE`, `TOK_JOIN`, `TOK_ON`, `TOK_AS`
  - `TOK_INT_TYPE`, `TOK_FLOAT_TYPE`, `TOK_VARCHAR`
  - `TOK_IDENT`, `TOK_INT_LIT`, `TOK_FLOAT_LIT`, `TOK_STR_LIT`
  - `TOK_EQ`, `TOK_NEQ`, `TOK_GT`, `TOK_LT`, `TOK_GTE`, `TOK_LTE`
  - `TOK_DOT`, `TOK_COMMA`, `TOK_SEMI`, `TOK_LPAREN`, `TOK_RPAREN`, `TOK_STAR`
- Expose `yylineno` (enabled via `%option yylineno` in the `.l` file) for error messages in the parser.

---

### Thanos — Parser (Bison) & Syntax Error Reporting

**Core file:** `parser.y`

#### Tasks

**Q1a (co-lead with Giorgos):** Draft the BNF grammar — Thanos owns the *non-terminal* (structural) side: how tokens combine into statements.

**Q1b — Bison parser:** Implement `parser.y` with grammar rules for:

- **CREATE TABLE statement:**
  ```
  CREATE TABLE ident ( col_def_list ) ;
  col_def_list → col_def | col_def_list , col_def
  col_def      → ident type
  type         → INT | FLOAT | VARCHAR ( int_lit )
  ```

- **SELECT statement** (clause order enforced):
  ```
  SELECT col_list FROM ident
    [ join_clause* ]
    [ WHERE condition ]
    [ GROUP BY col_list ]
    [ ORDER BY col_list ]
    [ LIMIT int_lit ]
  ;
  ```

- **Condition expressions** with proper precedence (NOT > AND > OR):
  ```
  condition → condition OR condition
            | condition AND condition
            | NOT condition
            | expr op expr
            | expr [ NOT ] IN ( literal_list )
            | ( condition )
  ```

- **Column references:** both `col` and `table.col` (dotted notation, needed for Q3/JOIN).

- **Output behavior:**
  - Print each input line as it is parsed (echo to stdout).
  - On success: print `Syntactic analysis successful.`
  - On error: print `Syntax error on line N: <description>` and stop.

**Q3a — JOIN syntax:**
```
join_clause → JOIN ident ON table_col = table_col
table_col   → ident . ident
```
- Add 0 or more JOIN clauses after the FROM clause.
- Wire semantic hooks: call Nasos's functions from Bison action blocks.

#### Interfaces to Nasos

Call these functions from Bison action blocks (defined in `semantics.h`):

```c
// In CREATE TABLE action:
sem_create_table(table_name, columns, col_count);

// In SELECT action:
sem_from_check(table_name);
sem_column_exists(table_name, col_name);
sem_type_check(table_name, col_name, literal_type);

// In Q3 JOIN action:
sem_register_alias(alias, table_name);   // when AS is used
sem_resolve_alias(alias);                // to look up alias → table
```

These functions return `0` on success or `-1` on error (and print an error message internally).

---

### Nasos — Semantic Checks (C/C++) + Aliases + Testing + Report

**Core files:** `semantics.h`, `semantics.c`, test `.sql` files, final report PDF

#### Tasks

**Q2 — Symbol table + semantic checks:**

Implement a symbol table in C:

```c
// Internal structure (in semantics.c):
typedef struct {
    char name[64];
    ColType type;
    int varchar_n;   // only used when type == TYPE_VARCHAR
} Column;

typedef struct {
    char name[64];
    Column cols[64];
    int col_count;
} Table;

Table symbol_table[128];
int table_count = 0;
```

Semantic rules to enforce:

| Check | Rule |
|-------|------|
| Q2a | Reject CREATE if table name already exists in symbol table |
| Q2b | Reject FROM/JOIN if table name does not exist in symbol table |
| Q2c | Reject CREATE if two columns in the same table have the same name (same name in different tables is OK) |
| Q2d | Reject SELECT/WHERE/GROUP BY/ORDER BY column if it does not exist in the referenced table |
| Q2e | Type compatibility in WHERE conditions (see below) |

**Type compatibility rules (Q2e):**

| Column type | Compatible literal |
|-------------|-------------------|
| INT | integer literal only |
| FLOAT | integer literal or float literal |
| VARCHAR(n) | string literal only |
| (NOT) IN list | every literal in the list must match column type |

**Q3b — Alias support:**

- Per-query alias map: `alias_name → table_name`
- `sem_register_alias(alias, table)`: store the mapping; error if alias already defined in this query
- `sem_resolve_alias(alias)`: return the real table name
- When a table is given an alias (`FROM Students AS s`), column references must use the alias (`s.id`), not the original table name
- Reset alias map at the start of each new SELECT query

**Test files (create one `.sql` file per category):**

| File | Tests |
|------|-------|
| `test_create_valid.sql` | Valid CREATE TABLE statements |
| `test_create_invalid.sql` | Duplicate table name, duplicate column name |
| `test_select_valid.sql` | SELECT with all optional clauses, multiple conditions |
| `test_select_invalid.sql` | Undefined table, undefined column, type mismatch, IN type error |
| `test_join_valid.sql` | JOIN with valid tables and column references |
| `test_join_invalid.sql` | JOIN referencing undefined table or undefined column |
| `test_alias_valid.sql` | Alias defined and used correctly |
| `test_alias_invalid.sql` | Alias inconsistency, alias shadowing |

**Written report (submit as PDF):**

1. Team info (names, student IDs, contribution summary)
2. BNF grammar (copy from Giorgos + Thanos's final version)
3. Final `lexer.l` and `parser.y` (paste full source)
4. Final `semantics.h` / `semantics.c` (paste full source)
5. Screenshots of test runs (one per test file, showing correct output)
6. Assumptions and design notes (any ambiguities you resolved)

---

## Shared Header: `semantics.h`

Giorgos and Thanos must include this header. Nasos implements it in `semantics.c`.

```c
#ifndef SEMANTICS_H
#define SEMANTICS_H

typedef enum { TYPE_INT, TYPE_FLOAT, TYPE_VARCHAR } ColType;

typedef struct {
    char name[64];
    ColType type;
    int varchar_n;
} Column;

typedef struct {
    char name[64];
    Column cols[64];
    int col_count;
} Table;

/* Called from Bison actions — return 0 on success, -1 on error */
int sem_create_table(const char *name, Column *cols, int col_count);
int sem_from_check(const char *table_name);
int sem_column_exists(const char *table_name, const char *col_name);
int sem_type_check(const char *table_name, const char *col_name, ColType lit_type);

/* Alias support (Q3b) */
int sem_register_alias(const char *alias, const char *table_name);
const char *sem_resolve_alias(const char *alias);
void sem_reset_aliases(void);   /* call at the start of each SELECT */

#endif /* SEMANTICS_H */
```

**Important:** Nasos should provide a stub implementation (all functions return 0) early on so Giorgos and Thanos can compile and test without waiting for Q2 to be complete.

---

## Suggested Timeline

| Week | Dates | Milestone |
|------|-------|-----------|
| 1 | Now → Apr 6 | All three read the full assignment PDF; Giorgos + Thanos draft BNF together; Nasos sets up stub `semantics.h/.c` |
| 2 | Apr 7–13 | Giorgos: Flex lexer compiling with all tokens; Thanos: skeleton Bison grammar compiling (no actions yet) |
| 3 | Apr 14–20 | Thanos: full Q1 parser with echo + error output; Nasos: symbol table + Q2a/b working |
| 4 | Apr 21–27 | Nasos: Q2c/d/e complete; Giorgos: polish lexer edge cases, verify Q3 keywords work |
| 5 | Apr 28 – May 4 | Thanos: Q3a JOIN syntax; Nasos: Q3b alias semantics |
| 6 | May 5–18 | Integration, testing, bug fixing across all components |
| 7 | May 19–28 | Nasos: write all test files + compile report; full team: review, screenshots, final check |
| 8 | May 31 | Submit on eclass |

---

## Build Instructions

```makefile
# Makefile (suggested)
myParser: lexer.l parser.y semantics.c semantics.h
	bison -d parser.y -o parser.tab.c
	flex -o lexer.yy.c lexer.l
	gcc -o myParser parser.tab.c lexer.yy.c semantics.c -lfl

clean:
	rm -f myParser parser.tab.c parser.tab.h lexer.yy.c
```

Run the parser:
```
./myParser input.sql
```

---

## Verification Checklist

### Q1 — Syntax

- [ ] `./myParser valid_query.sql` → prints program + `Syntactic analysis successful.`
- [ ] `./myParser bad_syntax.sql` → prints up to error line + `Syntax error on line N`
- [ ] Keywords are case-insensitive: `SeLeCt`, `FROM`, `from` all work
- [ ] Identifiers are case-sensitive: `Students` ≠ `students`
- [ ] Single-line comments (`--`) are ignored mid-statement
- [ ] Multi-line comments (`/* */`) are ignored mid-statement
- [ ] Line numbers in error messages are correct for multi-line input
- [ ] `SELECT *` works
- [ ] `NOT IN (...)` list parsed correctly
- [ ] All optional SELECT clauses can be omitted or combined

### Q2 — Semantics

- [ ] CREATE duplicate table name → semantic error
- [ ] CREATE duplicate column name within same table → semantic error
- [ ] FROM referencing undefined table → semantic error
- [ ] SELECT/WHERE column not in table schema → semantic error
- [ ] INT column compared with string literal → type error
- [ ] FLOAT column compared with string literal → type error
- [ ] VARCHAR column compared with integer literal → type error
- [ ] `col IN (v1, v2)` where any `vN` has wrong type → type error

### Q3 — JOIN & Aliases

- [ ] JOIN with valid tables and columns → accepted
- [ ] JOIN referencing undefined table → semantic error
- [ ] JOIN referencing undefined column → semantic error
- [ ] `FROM t1 AS a JOIN t2 AS b ON a.id = b.id` → accepted
- [ ] Alias defined, column referenced without alias → semantic error
- [ ] Same alias defined twice in one query → semantic error

---

## Notes & Conventions

- Use `%option yylineno` in `lexer.l` — do not manually track line numbers.
- Bison `%left`/`%right` declarations handle operator precedence; declare `OR` < `AND` < `NOT`.
- The `yyerror` function in `parser.y` should print `Syntax error on line %d: %s\n` using `yylineno`.
- Nasos's semantic functions should print their own error messages to `stderr` before returning `-1`; Bison actions just need to check the return value and call `YYABORT` if needed.
- Agree on the exact output format early — the graders will diff your output against expected output.
- Commit to a shared git repo so you can see each other's work and avoid merge conflicts.
