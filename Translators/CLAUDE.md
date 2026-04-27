# mySQLq Compiler — Project Context

## Course
University of Patras — Translators (Spring 2026)
Assignment: `Askisi2026.pdf`
Team doc: `ASSIGNMENT.md`

## Project
Implement a subset SQL compiler (mySQLq) using **Flex + Bison + C**.

## Files
| File | Purpose |
|------|---------|
| `lexer.l` | Flex lexer — tokenises mySQLq input |
| `parser.y` | Bison parser — grammar + semantic call sites |
| `semantics.h` | Shared types + function declarations |
| `semantics.c` | Semantic analysis (symbol table, type checks) |
| `Makefile` | Build system |
| `tests/` | SQL test files (valid + invalid for each feature) |

## Build
```bash
make          # produces ./myParser
./myParser tests/test_select_valid.sql
```
On Windows use **WinFlexBison** + MinGW/MSYS2 (see Makefile notes).

## Supported SQL (subset)
- `CREATE TABLE` with INT / FLOAT / VARCHAR(n) columns
- `SELECT` with aliases (`AS`), multi-table `JOIN ... ON`, `WHERE` conditions,
  `GROUP BY`, `ORDER BY`, `LIMIT`
- WHERE: comparison operators, `IN (...)`, `NOT IN (...)`, `AND/OR/NOT`

## Key conventions
- Keywords are case-insensitive (Flex `%option case-insensitive`)
- Identifiers are case-sensitive at semantic level
- Semantic errors → stderr; syntax errors → stdout (for grader)
- `lines[]` array in `parser.y` stores source lines for error reporting

## Questions
- Q1: Lexer + Parser (lexer.l, parser.y)
- Q2: Semantic analysis (semantics.c)
- Q3: JOINs + aliases (extensions in all three files)
