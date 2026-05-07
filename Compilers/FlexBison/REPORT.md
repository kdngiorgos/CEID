# Αναφορά Εργασίας — SQL Compiler με Flex & Bison

**Μάθημα:** Μεταγλωττιστές  
**Φοιτητές:** Κάνδηλας Γεώργιος (ΑΜ: 1115510) · Καρπέτας Απόστολος (ΑΜ: 1115507)

---

## 1. Εισαγωγή

Η εργασία υλοποιεί έναν αναλυτή για υποσύνολο της γλώσσας SQL χρησιμοποιώντας:

- **Flex** — λεξιλογικός αναλυτής (lexer): διαβάζει χαρακτήρες και παράγει tokens
- **Bison** — συντακτικός αναλυτής (parser): δέχεται tokens και ελέγχει τη γραμματική

Η εργασία χωρίζεται σε τρία ερωτήματα, κάθε ένα από τα οποία επεκτείνει το προηγούμενο:

| Ερώτημα | Τι προσθέτει |
|---------|-------------|
| Q1 | Συντακτική ανάλυση (parsing) |
| Q2 | Σημασιολογική ανάλυση (symbol table, type checking) |
| Q3 | JOIN, aliases, qualified column references |

---

## 2. Αρχιτεκτονική — Flex + Bison

### Ροή εκτέλεσης

```
Αρχείο .sql
     │
     ▼
  lexer.l  (Flex)
  ─────────────────────────────────────────────
  Διαβάζει χαρακτήρες → επιστρέφει tokens
  π.χ. "SELECT" → TOK_SELECT
       "students" → TOK_IDENT("students")
       42 → TOK_INT_LIT(42)
     │
     ▼
  parser.y  (Bison)
  ─────────────────────────────────────────────
  Δέχεται tokens → ελέγχει γραμματική
  → εκτελεί semantic actions (Q2, Q3)
     │
     ▼
  Αποτέλεσμα: επιτυχία ή μήνυμα σφάλματος
```

### Αλληλεπίδραση Flex–Bison

Το Bison καλεί `yylex()` κάθε φορά που χρειάζεται το επόμενο token. Το Flex υλοποιεί αυτή τη συνάρτηση. Η επικοινωνία γίνεται μέσω της καθολικής μεταβλητής `yylval` (η οποία ορίζεται από το `%union` στο parser), όπου ο lexer αποθηκεύει την τιμή του token (π.χ. το string ενός identifier).

---

## 3. Γραμματική BNF

Παρακάτω παρουσιάζεται η πλήρης γραμματική για κάθε ερώτημα σε μορφή BNF.  
Σύμβολα: κεφαλαία = terminals (tokens), πεζά = non-terminals, `ε` = κενή παραγωγή.

---

### 3.1 Βασική Γραμματική (Q1 & Q2 — κοινή βάση)

```
stmt_list       →  ε
                |  stmt_list  stmt

stmt            →  create_stmt
                |  select_stmt

create_stmt     →  CREATE TABLE IDENT ( col_def_list ) ;

col_def_list    →  col_def
                |  col_def_list , col_def

col_def         →  IDENT type_spec
                |  IDENT VARCHAR ( INT_LIT )

type_spec       →  INT
                |  FLOAT

col_expr_list   →  *
                |  col_ref_list

col_ref_list    →  col_ref
                |  col_ref_list , col_ref

opt_where       →  ε
                |  WHERE condition

condition       →  condition OR  condition
                |  condition AND condition
                |  NOT condition
                |  ( condition )
                |  col_ref comp_op literal
                |  col_ref IN ( in_literal_list )
                |  col_ref NOT IN ( in_literal_list )

comp_op         →  =  |  !=  |  >  |  <  |  >=  |  <=

literal         →  INT_LIT
                |  FLOAT_LIT
                |  STR_LIT

in_literal_list →  literal
                |  in_literal_list , literal

opt_group       →  ε
                |  GROUP BY col_ref_list

opt_order       →  ε
                |  ORDER BY col_ref_list

opt_limit       →  ε
                |  LIMIT INT_LIT
```

---

### 3.2 Γραμματική Q1

Στο Q1 το `col_ref` υποστηρίζει και qualified αναφορές (`table.column`) και απλά ονόματα,
και το `select_stmt` αναφέρεται απευθείας σε `IDENT` πίνακα:

```
select_stmt     →  SELECT col_expr_list FROM IDENT
                       opt_where opt_group opt_order opt_limit ;

col_ref         →  IDENT
                |  IDENT . IDENT
```

---

### 3.3 Γραμματική Q2

Στο Q2 το `col_ref` επιστρέφει **μόνο** απλό `IDENT` — qualified αναφορές δεν υποστηρίζονται
επειδή δεν υπάρχουν JOINs/aliases. Εισάγεται ξεχωριστό `select_col_ref_list` για το deferred
checking των SELECT στηλών:

```
select_stmt          →  SELECT select_col_ref_list FROM IDENT
                             opt_where opt_group opt_order opt_limit ;
                      |  SELECT * FROM IDENT
                             opt_where opt_group opt_order opt_limit ;

select_col_ref_list  →  col_ref
                      |  select_col_ref_list , col_ref

col_ref              →  IDENT
```

> Αλλαγή από Q1: αφαιρείται η εναλλακτική `IDENT . IDENT` από το `col_ref`.

---

### 3.4 Γραμματική Q3

Στο Q3 επιστρέφει το `IDENT . IDENT` στο `col_ref` και προστίθενται οι κανόνες για
`JOIN`, `AS` και `qualified_col_ref`:

```
select_stmt          →  SELECT col_expr_list FROM table_ref join_list
                             opt_where opt_group opt_order opt_limit ;

table_ref            →  IDENT
                      |  IDENT AS IDENT

join_list            →  ε
                      |  join_list JOIN table_ref ON qualified_col_ref = qualified_col_ref

qualified_col_ref    →  IDENT . IDENT

col_ref              →  IDENT
                      |  IDENT . IDENT
```

> Αλλαγές από Q2:
> - `select_stmt` χρησιμοποιεί `table_ref join_list` αντί για `IDENT`
> - Νέο non-terminal `table_ref` για υποστήριξη `AS`
> - Νέο non-terminal `join_list` για έναν ή περισσότερους JOINs
> - Νέο non-terminal `qualified_col_ref` που απαιτεί υποχρεωτικά `table.column`
> - Επιστρέφει η εναλλακτική `IDENT . IDENT` στο `col_ref`

---

### 3.5 Σύνοψη Tokens (Lexer)

| Token | Τιμή |
|---|---|
| `IDENT` | Αλφαριθμητικό αναγνωριστικό (`[a-zA-Z_][a-zA-Z0-9_]*`) |
| `INT_LIT` | Ακέραιος αριθμός (`-?[0-9]+`) |
| `FLOAT_LIT` | Δεκαδικός (`-?[0-9]+.[0-9]+`) |
| `STR_LIT` | Αλφαριθμητικό σε single quotes (`'...'`) |
| Keywords | `SELECT FROM WHERE GROUP ORDER BY LIMIT CREATE TABLE JOIN ON AS IN AND OR NOT` |
| Types | `INT FLOAT VARCHAR` |
| Operators | `= != > < >= <=` |
| Punctuation | `, . ; ( ) *` |

---

## 4. Ερώτημα 1 — Συντακτική Ανάλυση

### Τι κάνει

Ο parser του Q1 ελέγχει **μόνο αν το SQL είναι συντακτικά σωστό**. Δεν θυμάται ποιοι πίνακες ή στήλες έχουν οριστεί — απλώς επαληθεύει ότι η δομή ακολουθεί τη γραμματική.

### Γραμματική

Υποστηρίζονται δύο τύποι εντολών:

```
CREATE TABLE <name> ( <col_def_list> ) ;
SELECT <col_expr_list> FROM <table> [WHERE ...] [GROUP BY ...] [ORDER BY ...] [LIMIT n] ;
```

### Βασικές σχεδιαστικές επιλογές

#### Γιατί το `program` είναι σχολιασμένο

```bison
/* non-terminals

program
    : stmt_list
    ;

/* eite adeio eite exei statement */
stmt_list
    ...
```

Το block comment ξεκινά στη γραμμή `/* non-terminals` και κλείνει στο `*/` στο τέλος του `/* eite adeio... */`. Αυτό **σχολιάζει εκτός τον ορισμό του `program`**, οπότε το Bison χρησιμοποιεί ως αρχικό σύμβολο το `stmt_list` (ο πρώτος κανόνας που "βλέπει"). Αποτέλεσμα: το πρόγραμμα μπορεί να είναι **κενό** (zero statements), κάτι επιθυμητό.

#### Γιατί χρησιμοποιείται `%left` / `%right` για τους λογικούς τελεστές

```bison
%left  TOK_OR
%left  TOK_AND
%right TOK_NOT
```

Το Bison από μόνο του δεν ξέρει ποιος τελεστής δεσμεύει περισσότερο. Χωρίς αυτές τις δηλώσεις, η έκφραση `a AND b OR c` θα ήταν αμφίσημη. Ορίζουμε:
- `OR` έχει τη **χαμηλότερη** προτεραιότητα (δηλώνεται πρώτο)
- `AND` έχει μεσαία
- `NOT` έχει την **υψηλότερη** και είναι δεξιά προσεταιριστικό (`%right`) επειδή είναι μοναδιαίος τελεστής

Έτσι `a AND b OR c` παρσάρεται σωστά ως `(a AND b) OR c`.

#### Γιατί το `col_ref` επιστρέφει string (`%type <str>`)

```bison
col_ref
    : TOK_IDENT { $$ = strdup($1); }
    | TOK_IDENT '.' TOK_IDENT
        {
            int len = strlen($1) + 1 + strlen($3) + 1;
            $$ = malloc(len);
            snprintf($$, len, "%s.%s", $1, $3);
        }
    ;
```

Το `col_ref` είναι το **μόνο non-terminal με σημασιολογική τιμή** στο Q1. Επιστρέφει string ώστε οι κανόνες που το χρησιμοποιούν να μπορούν να πάρουν το όνομα της στήλης — αυτό θα χρειαστεί στα Q2/Q3 για τους semantic checks. Στο Q1 δεν χρησιμοποιείται η τιμή, αλλά η δομή παραμένει για συμβατότητα.

#### Γιατί το float pattern έρχεται πριν το int στον lexer

```flex
"-"?[0-9]+"."[0-9]+  { yylval.fval = atof(yytext); return TOK_FLOAT_LIT; }
"-"?[0-9]+           { yylval.ival = atoi(yytext); return TOK_INT_LIT; }
```

Ο Flex επιλέγει τον **πρώτο κανόνα που ταιριάζει** σε περίπτωση ισοπαλίας μήκους. Αν το int pattern ήταν πρώτο, το `3.14` θα διαβαζόταν ως `3` (int) και `.` (punctuation) χωριστά. Το float pattern πρέπει να έρχεται πρώτο.

#### Γιατί τα διμερή operators (`!=`, `>=`) έρχονται πριν τα μοναδιαία (`=`, `>`)

Ίδια λογική: ο Flex ταιριάζει με το **μεγαλύτερο δυνατό string** (maximal munch), αλλά αν δύο κανόνες έχουν ίδιο μήκος match, κερδίζει ο πρώτος. Αν το `=` ήταν πριν το `>=`, τότε το `>=` θα διαβαζόταν ως `>` + `=`.

---

## 5. Ερώτημα 2 — Σημασιολογική Ανάλυση

### Τι προσθέτει στο Q1

Το Q2 εισάγει **σημασιολογικούς ελέγχους** μέσω ενός symbol table. Δεν αρκεί πλέον να είναι συντακτικά σωστό το SQL — πρέπει να είναι και **σημασιολογικά έγκυρο**.

### Symbol Table

```c
static Table symbol_table[MAX_TABLES];
static int   table_count = 0;
```

Ένας στατικός πίνακας από `Table` structs. Κάθε `Table` περιέχει το όνομά του και τις στήλες του. Γεμίζει κάθε φορά που παρσάρεται ένα `CREATE TABLE`.

```c
typedef struct {
    char name[64];
    ColType type;
    int varchar_size;
} Column;

typedef struct {
    char name[64];
    Column cols[MAX_COLS];
    int col_count;
} Table;
```

### Τύποι στηλών — ColType

```c
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_VARCHAR,
    TYPE_UNKNOWN
} ColType;
```

Χρησιμοποιείται `enum` αντί για raw integers για αναγνωσιμότητα. Η τιμή `TYPE_UNKNOWN` χρησιμεύει ως sentinel για αρχικοποίηση.

### Έλεγχος τύπων — Γιατί το FLOAT δέχεται INT literal

```c
if (c->type == TYPE_INT && lit_type == TYPE_INT)
    ok = 1;
else if (c->type == TYPE_FLOAT && (lit_type == TYPE_INT || lit_type == TYPE_FLOAT))
    ok = 1;
else if (c->type == TYPE_VARCHAR && lit_type == TYPE_VARCHAR)
    ok = 1;
```

Στη SQL (και στους περισσότερους γλωσσικούς τύπους), ένας ακέραιος `42` μπορεί να συγκριθεί με FLOAT column χωρίς πρόβλημα — γίνεται implicit **widening** (διεύρυνση τύπου). Το αντίστροφο δεν ισχύει: `3.14 > int_column` δεν επιτρέπεται.

### Deferred column checking — Γιατί τα SELECT cols ελέγχονται μετά το FROM

```c
/* ta cols tou SELECT ta apothikevw edw kai ta elegxo meta to FROM
   giati o parser ta vlepei prin diabasei to table */
static char *select_cols[MAX_SELECT_COLS];
```

Ο parser διαβάζει τις στήλες του `SELECT` **πριν** φτάσει στο `FROM`. Δεν ξέρει ακόμα ποιος πίνακας είναι active, άρα δεν μπορεί να ελέγξει αν υπάρχουν. Η λύση: αποθηκεύουμε τις στήλες σε buffer και τις ελέγχουμε **αφού** επεξεργαστούμε το `FROM`:

```bison
select_stmt
    : TOK_SELECT
        { select_col_count = 0; }        /* αρχικοποίηση buffer */
      col_expr_list TOK_FROM TOK_IDENT
        {
            sem_from_check($5);           /* ελέγχουμε τον πίνακα */
            current_query_table = strdup($5);
        }
      opt_where opt_group opt_order opt_limit ';'
        {
            /* τώρα ελέγχουμε τις στήλες */
            for (int i = 0; i < select_col_count; i++)
                sem_column_exists(current_query_table, select_cols[i]);
        }
    ;
```

### Γιατί το col_ref στο Q2 δεν έχει `table.column`

Στο Q2 δεν υπάρχουν JOINs ή aliases — κάθε query αναφέρεται σε έναν μόνο πίνακα. Το `table.column` δεν έχει νόημα χωρίς δύο πίνακες. Προστίθεται στο Q3.

---

## 6. Ερώτημα 3 — JOIN, Aliases, Qualified References

### Τι προσθέτει στο Q2

| Χαρακτηριστικό | Περιγραφή |
|---|---|
| `JOIN ... ON` | Σύνδεση δύο πινάκων |
| `AS` | Alias για πίνακα |
| `table.column` | Qualified αναφορά στήλης |
| Ambiguity check | Προειδοποίηση αν bare στήλη είναι αμφίσημη |
| Alias resolution | Αυτόματη μετατροπή alias → πραγματικό όνομα |

### Alias Map

```c
typedef struct {
    char alias[64];
    char table[64];
} AliasEntry;

static AliasEntry alias_map[MAX_ALIASES];
static int        alias_count = 0;
```

Κάθε φορά που παρσάρεται `Students AS s`, καλείται `sem_register_alias("s", "Students")`. Η `sem_resolve_alias()` μετατρέπει ένα alias στο πραγματικό όνομα πίνακα:

```c
static const char *sem_resolve_alias(const char *name) {
    for (int i = 0; i < alias_count; i++)
        if (strcmp(alias_map[i].alias, name) == 0)
            return alias_map[i].table;
    return name; /* den einai alias, epistrefoume ws exei */
}
```

Αν το όνομα δεν είναι alias, επιστρέφεται ως έχει — έτσι η συνάρτηση δουλεύει και για απλά ονόματα πινάκων χωρίς alias.

### Γιατί το ON clause απαιτεί qualified references

```bison
join_list
    : join_list TOK_JOIN table_ref TOK_ON qualified_col_ref TOK_EQ qualified_col_ref
```

Η `qualified_col_ref` δέχεται **μόνο** `table.column`, όχι bare `column`. Αυτό επιβλήθηκε μετά από διευκρίνιση της άσκησης: στο `ON` clause πρέπει να είναι σαφές σε ποιον πίνακα ανήκει κάθε στήλη (π.χ. `s.student_id = e.student_id`). Ένα bare `student_id` θα ήταν αμφίσημο όταν και οι δύο πίνακες έχουν την ίδια στήλη.

### Active Tables και Deferred SELECT — Επέκταση του Q2 mechanism

Στο Q3 δεν υπάρχει ένας μόνο `current_query_table` — μπορεί να υπάρχουν πολλοί μετά από JOINs. Χρησιμοποιείται λίστα:

```c
static const char *query_tables[MAX_QUERY_TABLES];
static int         query_table_count = 0;
```

Γεμίζει **progressively** καθώς παρσάρονται τα `table_ref` και `join_list`. Η `sem_set_query_tables()` καλείται αφού φορτωθούν όλα, και μετά ελέγχονται οι deferred SELECT στήλες:

```bison
col_expr_list TOK_FROM table_ref join_list
    {
        sem_set_query_tables(query_tables, query_table_count);
    }
opt_where ... ';'
    {
        /* elegxos ton SELECT cols - deferred */
        for (int i = 0; i < select_col_count; i++) {
            char *dot = strchr(select_cols[i], '.');
            if (dot) {
                *dot = '\0';
                sem_column_in_query(select_cols[i], dot + 1);
            } else {
                sem_column_in_query(NULL, select_cols[i]);
            }
        }
    }
```

### Ambiguity Check

```c
if (found == 0)
    fprintf(stderr, "Semantic error: column '%s' not found in any active table.\n", col);
else if (found > 1)
    fprintf(stderr, "Semantic warning: column '%s' is ambiguous (found in %d tables).\n",
            col, found);
```

Αν μια bare στήλη (π.χ. `grade`) βρεθεί σε **περισσότερους από έναν** πίνακες, εκδίδεται warning. Ο χρήστης πρέπει να χρησιμοποιήσει qualified reference (π.χ. `e.grade`).

### Reset aliases μεταξύ SELECT statements

```c
static void sem_reset_aliases(void) {
    alias_count        = 0;
    active_table_count = 0;
}
```

Καλείται στην αρχή κάθε `select_stmt`. Τα aliases ισχύουν **μόνο** εντός του query που τα ορίζει — διαφορετικό query μπορεί να χρησιμοποιήσει το ίδιο alias για διαφορετικό πίνακα.

---

## 7. Κοινά Χαρακτηριστικά και στα Τρία Ερωτήματα

### Buffering γραμμών και εκτύπωση σφαλμάτων

```c
static char *lines[MAX_LINES];
static int   line_count = 0;
```

Πριν ξεκινήσει το parsing, ο `main()` διαβάζει ολόκληρο το αρχείο γραμμή-γραμμή στο buffer `lines[]`. Αυτό γίνεται γιατί η `yyerror()` πρέπει να εκτυπώσει τις γραμμές μέχρι το σφάλμα — αλλά μέχρι να καληφθεί η `yyerror()`, ο lexer έχει ήδη καταναλώσει τις γραμμές από το αρχείο. Λύση: διπλό πέρασμα με `rewind()`.

```c
/* 1ο πέρασμα: φόρτωση γραμμών */
while (fgets(buf, sizeof(buf), f) && line_count < MAX_LINES)
    lines[line_count++] = strdup(buf);

rewind(f);   /* επιστροφή στην αρχή */
yyin = f;    /* δίνουμε το αρχείο στον lexer */

/* 2ο πέρασμα: parsing */
int result = yyparse();
```

### Γιατί τα σχόλια SQL αγνοούνται στον lexer

```flex
"/*"([^*]|\*+[^*/])*\*+"/"   { /* skip */ }
"--"[^\n]*                    { /* skip */ }
```

Το regex για block comments είναι πιο πολύπλοκο επειδή πρέπει να αποφύγει false matches: `[^*]` ή `\*+[^*/]` σημαίνει "αστερίσκοι που δεν ακολουθούνται από `/`". Αν χρησιμοποιούσαμε απλά `"/*".*"*/"` θα υπήρχε πρόβλημα με nested ή multiline comments.

### Γιατί τα string literals αφαιρούν τα quotes

```flex
'[^']*'  {
    int len = strlen(yytext) - 2;  /* αφαιρούμε τα 2 single quotes */
    yylval.str = (char *)malloc(len + 1);
    strncpy(yylval.str, yytext + 1, len);  /* ξεκινάμε από +1 */
    yylval.str[len] = '\0';
    return TOK_STR_LIT;
}
```

Το `yytext` περιέχει `'hello'` (με quotes). Στο parser χρειαζόμαστε μόνο `hello`. Αφαιρούμε 2 χαρακτήρες από το μήκος και ξεκινάμε την αντιγραφή από τη θέση `+1`.

---

## 8. Σύγκριση των Τριών Ερωτημάτων

| | Q1 | Q2 | Q3 |
|---|:---:|:---:|:---:|
| Lexer tokens | ✓ | ✓ | ✓ + JOIN/ON/AS |
| Συντακτική ανάλυση | ✓ | ✓ | ✓ |
| Symbol table | ✗ | ✓ | ✓ |
| Έλεγχος ύπαρξης πίνακα | ✗ | ✓ | ✓ |
| Έλεγχος ύπαρξης στήλης | ✗ | ✓ | ✓ |
| Type checking | ✗ | ✓ | ✓ |
| JOIN support | ✗ | ✗ | ✓ |
| Aliases (AS) | ✗ | ✗ | ✓ |
| Qualified `table.col` | ✓ (μόνο parsing) | ✗ | ✓ (με έλεγχο) |
| Ambiguity check | ✗ | ✗ | ✓ |
| Alias resolution | ✗ | ✗ | ✓ |

---

## 9. Παράδειγμα Εκτέλεσης (Q3)

Δεδομένο input:

```sql
CREATE TABLE Students (student_id INT, gpa FLOAT);
CREATE TABLE Enrollments (enrollment_id INT, student_id INT, grade VARCHAR(2));

SELECT s.student_id, e.grade
FROM Students AS s
JOIN Enrollments AS e ON s.student_id = e.student_id
WHERE s.gpa >= 3.0
  AND e.grade NOT IN ('F', 'D');
```

Ροή ελέγχων:

1. `CREATE TABLE Students` → προστίθεται στο symbol table
2. `CREATE TABLE Enrollments` → προστίθεται στο symbol table
3. `FROM Students AS s` → `sem_from_check("Students")` ✓ · εγγραφή alias `s→Students`
4. `JOIN Enrollments AS e ON s.student_id = e.student_id`
   - `sem_from_check("Enrollments")` ✓
   - εγγραφή alias `e→Enrollments`
   - `sem_column_exists("s"→"Students", "student_id")` ✓
   - `sem_column_exists("e"→"Enrollments", "student_id")` ✓
5. `sem_set_query_tables(["s","e"], 2)`
6. Deferred SELECT cols: `s.student_id` → `sem_column_in_query("s","student_id")` ✓ · `e.grade` ✓
7. WHERE: `s.gpa >= 3.0` → `sem_type_check("Students","gpa", FLOAT)` ✓
8. `e.grade NOT IN ('F','D')` → `sem_type_check_in("Enrollments","grade", [VARCHAR,VARCHAR])` ✓

**Αποτέλεσμα:** Εκτύπωση του αρχείου + `Syntactic analysis successful.`
