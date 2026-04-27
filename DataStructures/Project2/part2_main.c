/*
 * Project 2 - Μέρος ΙΙ
 * Ενιαίο πρόγραμμα: AVL BST + Hash Table με Chaining
 *
 * Επιλογές φόρτωσης:
 *   A) AVL ταξινομημένο κατά Date
 *      Μενού: εμφάνιση (inorder), αναζήτηση, τροποποίηση, διαγραφή
 *   B) AVL ταξινομημένο κατά Cumulative
 *      Μενού: εύρεση ελαχίστου/μεγίστου Cumulative
 *   C) Hash Table με Chaining (hash = ASCII sum mod m)
 *      Μενού: αναζήτηση, τροποποίηση, διαγραφή κατά Date
 *
 * Αρχείο δεδομένων: covid_trade.csv (στον ίδιο φάκελο)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATE    16
#define MAX_LINE   512
#define MAX_RECORDS 120000
#define HASH_M     100003   /* αριθμός buckets (πρώτος αριθμός) */

/* ================================================================
 * Δομές δεδομένων
 * ================================================================ */

typedef struct AVLNode {
    char  date[MAX_DATE];
    long  cumulative;
    int   height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

typedef struct HashNode {
    char  date[MAX_DATE];
    long  cumulative;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode **buckets;
    int m;
} HashTable;

/* ================================================================
 * Ανάγνωση CSV
 * ================================================================ */

typedef struct {
    char date[MAX_DATE];
    long cumulative;
} Record;

int readCSV(const char *filename, Record *records) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Σφάλμα: δεν βρέθηκε το αρχείο %s\n", filename);
        return 0;
    }
    char line[MAX_LINE];
    int count = 0;
    fgets(line, MAX_LINE, fp);
    while (fgets(line, MAX_LINE, fp) && count < MAX_RECORDS) {
        char direction[32], weekday[16], country[64];
        char commodity[64], transport[32], measure[32];
        int  year;
        long value, cumulative;
        char date[MAX_DATE];
        if (sscanf(line, "%31[^,],%d,%15[^,],%15[^,],%63[^,],%63[^,],%31[^,],%31[^,],%ld,%ld",
                   direction, &year, date, weekday, country, commodity,
                   transport, measure, &value, &cumulative) == 10) {
            strncpy(records[count].date, date, MAX_DATE - 1);
            records[count].cumulative = cumulative;
            count++;
        }
    }
    fclose(fp);
    return count;
}

/* ================================================================
 * AVL Tree (ταξινόμηση κατά Date)
 * ================================================================ */

static int avlHeight(AVLNode *n) { return n ? n->height : 0; }

static int avlMax(int a, int b) { return a > b ? a : b; }

static void avlUpdateHeight(AVLNode *n) {
    if (n) n->height = 1 + avlMax(avlHeight(n->left), avlHeight(n->right));
}

static int avlBalance(AVLNode *n) {
    return n ? avlHeight(n->left) - avlHeight(n->right) : 0;
}

static AVLNode* avlRotateRight(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;
    x->right = y;
    y->left  = T2;
    avlUpdateHeight(y);
    avlUpdateHeight(x);
    return x;
}

static AVLNode* avlRotateLeft(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;
    y->left  = x;
    x->right = T2;
    avlUpdateHeight(x);
    avlUpdateHeight(y);
    return y;
}

static AVLNode* avlNewNode(const char *date, long cumulative) {
    AVLNode *n = (AVLNode*)malloc(sizeof(AVLNode));
    strncpy(n->date, date, MAX_DATE - 1);
    n->date[MAX_DATE-1] = '\0';
    n->cumulative = cumulative;
    n->height = 1;
    n->left = n->right = NULL;
    return n;
}

static AVLNode* avlBalance_node(AVLNode *n) {
    avlUpdateHeight(n);
    int bal = avlBalance(n);

    /* LL */
    if (bal > 1 && avlBalance(n->left) >= 0)
        return avlRotateRight(n);
    /* LR */
    if (bal > 1 && avlBalance(n->left) < 0) {
        n->left = avlRotateLeft(n->left);
        return avlRotateRight(n);
    }
    /* RR */
    if (bal < -1 && avlBalance(n->right) <= 0)
        return avlRotateLeft(n);
    /* RL */
    if (bal < -1 && avlBalance(n->right) > 0) {
        n->right = avlRotateRight(n->right);
        return avlRotateLeft(n);
    }
    return n;
}

AVLNode* avlInsertDate(AVLNode *root, const char *date, long cumulative) {
    if (!root) return avlNewNode(date, cumulative);
    int cmp = strcmp(date, root->date);
    if (cmp < 0)      root->left  = avlInsertDate(root->left,  date, cumulative);
    else if (cmp > 0) root->right = avlInsertDate(root->right, date, cumulative);
    else              return root; /* duplicate date: skip */
    return avlBalance_node(root);
}

AVLNode* avlSearchDate(AVLNode *root, const char *date) {
    if (!root) return NULL;
    int cmp = strcmp(date, root->date);
    if (cmp == 0) return root;
    if (cmp < 0)  return avlSearchDate(root->left,  date);
    return            avlSearchDate(root->right, date);
}

static AVLNode* avlMinNode(AVLNode *root) {
    while (root->left) root = root->left;
    return root;
}

AVLNode* avlDeleteDate(AVLNode *root, const char *date) {
    if (!root) return NULL;
    int cmp = strcmp(date, root->date);
    if (cmp < 0)      root->left  = avlDeleteDate(root->left,  date);
    else if (cmp > 0) root->right = avlDeleteDate(root->right, date);
    else {
        if (!root->left || !root->right) {
            AVLNode *tmp = root->left ? root->left : root->right;
            free(root);
            return tmp;
        }
        AVLNode *succ = avlMinNode(root->right);
        strncpy(root->date, succ->date, MAX_DATE);
        root->cumulative = succ->cumulative;
        root->right = avlDeleteDate(root->right, succ->date);
    }
    return avlBalance_node(root);
}

void avlInorder(AVLNode *root, int *count) {
    if (!root) return;
    avlInorder(root->left, count);
    (*count)++;
    if (*count <= 20 || *count % 5000 == 0)
        printf("  %-15s  %ld\n", root->date, root->cumulative);
    avlInorder(root->right, count);
}

/* AVL ταξινομημένο κατά Cumulative */
AVLNode* avlInsertCumul(AVLNode *root, const char *date, long cumulative) {
    if (!root) return avlNewNode(date, cumulative);
    if (cumulative < root->cumulative)
        root->left  = avlInsertCumul(root->left,  date, cumulative);
    else
        root->right = avlInsertCumul(root->right, date, cumulative);
    return avlBalance_node(root);
}

AVLNode* avlMinCumul(AVLNode *root) {
    if (!root) return NULL;
    while (root->left) root = root->left;
    return root;
}

AVLNode* avlMaxCumul(AVLNode *root) {
    if (!root) return NULL;
    while (root->right) root = root->right;
    return root;
}

/* ================================================================
 * Hash Table με Chaining
 * hash(date) = (ASCII sum of chars) mod m
 * ================================================================ */

int hashFunction(const char *date, int m) {
    unsigned long sum = 0;
    int i;
    for (i = 0; date[i]; i++) sum += (unsigned char)date[i];
    return (int)(sum % m);
}

HashTable* hashCreate(int m) {
    HashTable *ht = (HashTable*)malloc(sizeof(HashTable));
    ht->m = m;
    ht->buckets = (HashNode**)calloc(m, sizeof(HashNode*));
    return ht;
}

void hashInsert(HashTable *ht, const char *date, long cumulative) {
    int idx = hashFunction(date, ht->m);
    /* Έλεγχος αν υπάρχει ήδη */
    HashNode *cur = ht->buckets[idx];
    while (cur) {
        if (strcmp(cur->date, date) == 0) {
            cur->cumulative = cumulative;
            return;
        }
        cur = cur->next;
    }
    HashNode *node = (HashNode*)malloc(sizeof(HashNode));
    strncpy(node->date, date, MAX_DATE - 1);
    node->date[MAX_DATE-1] = '\0';
    node->cumulative = cumulative;
    node->next = ht->buckets[idx];
    ht->buckets[idx] = node;
}

HashNode* hashSearch(HashTable *ht, const char *date) {
    int idx = hashFunction(date, ht->m);
    HashNode *cur = ht->buckets[idx];
    while (cur) {
        if (strcmp(cur->date, date) == 0) return cur;
        cur = cur->next;
    }
    return NULL;
}

int hashModify(HashTable *ht, const char *date, long newCumulative) {
    HashNode *node = hashSearch(ht, date);
    if (!node) return 0;
    node->cumulative = newCumulative;
    return 1;
}

int hashDelete(HashTable *ht, const char *date) {
    int idx = hashFunction(date, ht->m);
    HashNode *cur = ht->buckets[idx];
    HashNode *prev = NULL;
    while (cur) {
        if (strcmp(cur->date, date) == 0) {
            if (prev) prev->next = cur->next;
            else      ht->buckets[idx] = cur->next;
            free(cur);
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

/* ================================================================
 * Μενού
 * ================================================================ */

void menuAVLDate(AVLNode **root) {
    int choice;
    char date[MAX_DATE];
    long newCumul;
    AVLNode *found;
    do {
        printf("\n--- Μενού AVL (ταξινόμηση κατά Date) ---\n");
        printf("1. Εμφάνιση δέντρου (inorder)\n");
        printf("2. Αναζήτηση Cumulative κατά Date\n");
        printf("3. Τροποποίηση Cumulative κατά Date\n");
        printf("4. Διαγραφή εγγραφής κατά Date\n");
        printf("5. Έξοδος\n");
        printf("Επιλογή: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1: {
            int count = 0;
            printf("\n%-15s  %s\n", "Ημερομηνία", "Cumulative");
            printf("------------------------------\n");
            avlInorder(*root, &count);
            printf("(Σύνολο: %d εγγραφές)\n", count);
            break;
        }
        case 2:
            printf("Δώσε ημερομηνία (DD/MM/YYYY): ");
            scanf("%s", date);
            found = avlSearchDate(*root, date);
            if (found)
                printf("Cumulative για %s: %ld\n", date, found->cumulative);
            else
                printf("Δεν βρέθηκε η ημερομηνία %s\n", date);
            break;
        case 3:
            printf("Δώσε ημερομηνία (DD/MM/YYYY): ");
            scanf("%s", date);
            found = avlSearchDate(*root, date);
            if (found) {
                printf("Τρέχον Cumulative: %ld\nΝέο Cumulative: ", found->cumulative);
                scanf("%ld", &newCumul);
                found->cumulative = newCumul;
                printf("Ενημερώθηκε.\n");
            } else {
                printf("Δεν βρέθηκε.\n");
            }
            break;
        case 4:
            printf("Δώσε ημερομηνία (DD/MM/YYYY): ");
            scanf("%s", date);
            *root = avlDeleteDate(*root, date);
            printf("Η εγγραφή διαγράφηκε (αν υπήρχε).\n");
            break;
        case 5:
            printf("Επιστροφή στο κύριο μενού.\n");
            break;
        default:
            printf("Άκυρη επιλογή.\n");
        }
    } while (choice != 5);
}

void menuAVLCumul(AVLNode *root) {
    int choice;
    AVLNode *node;
    do {
        printf("\n--- Μενού AVL (ταξινόμηση κατά Cumulative) ---\n");
        printf("1. Εύρεση ημερομηνίας(-ών) με ΕΛΑΧΙΣΤΟ Cumulative\n");
        printf("2. Εύρεση ημερομηνίας(-ών) με ΜΕΓΙΣΤΟ Cumulative\n");
        printf("3. Έξοδος\n");
        printf("Επιλογή: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            node = avlMinCumul(root);
            if (node) printf("Ελάχιστο Cumulative: %ld  (ημ/νία: %s)\n",
                             node->cumulative, node->date);
            else printf("Το δέντρο είναι άδειο.\n");
            break;
        case 2:
            node = avlMaxCumul(root);
            if (node) printf("Μέγιστο Cumulative: %ld  (ημ/νία: %s)\n",
                             node->cumulative, node->date);
            else printf("Το δέντρο είναι άδειο.\n");
            break;
        case 3:
            printf("Επιστροφή στο κύριο μενού.\n");
            break;
        default:
            printf("Άκυρη επιλογή.\n");
        }
    } while (choice != 3);
}

void menuHash(HashTable *ht) {
    int choice;
    char date[MAX_DATE];
    long newCumul;
    HashNode *found;
    do {
        printf("\n--- Μενού Hash Table ---\n");
        printf("1. Αναζήτηση Cumulative κατά Date\n");
        printf("2. Τροποποίηση Cumulative κατά Date\n");
        printf("3. Διαγραφή εγγραφής κατά Date\n");
        printf("4. Έξοδος\n");
        printf("Επιλογή: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            printf("Δώσε ημερομηνία (DD/MM/YYYY): ");
            scanf("%s", date);
            found = hashSearch(ht, date);
            if (found)
                printf("Cumulative για %s: %ld\n", date, found->cumulative);
            else
                printf("Δεν βρέθηκε η ημερομηνία %s\n", date);
            break;
        case 2:
            printf("Δώσε ημερομηνία (DD/MM/YYYY): ");
            scanf("%s", date);
            printf("Νέο Cumulative: ");
            scanf("%ld", &newCumul);
            if (hashModify(ht, date, newCumul))
                printf("Ενημερώθηκε.\n");
            else
                printf("Δεν βρέθηκε.\n");
            break;
        case 3:
            printf("Δώσε ημερομηνία (DD/MM/YYYY): ");
            scanf("%s", date);
            if (hashDelete(ht, date))
                printf("Η εγγραφή διαγράφηκε.\n");
            else
                printf("Δεν βρέθηκε.\n");
            break;
        case 4:
            printf("Επιστροφή στο κύριο μενού.\n");
            break;
        default:
            printf("Άκυρη επιλογή.\n");
        }
    } while (choice != 4);
}

/* ================================================================
 * Main
 * ================================================================ */

int main() {
    int loadChoice;
    printf("=== Project 2 - Μέρος ΙΙ ===\n\n");
    printf("Επιλογή δομής δεδομένων:\n");
    printf("  A) AVL BST ταξινομημένο κατά Date\n");
    printf("  B) AVL BST ταξινομημένο κατά Cumulative\n");
    printf("  C) Hash Table με Chaining\n");
    printf("Επιλογή (A/B/C): ");
    char choice[4];
    scanf("%3s", choice);

    printf("\nΑνάγνωση δεδομένων από covid_trade.csv...\n");
    Record *records = (Record*)malloc(MAX_RECORDS * sizeof(Record));
    int n = readCSV("covid_trade.csv", records);
    if (n == 0) { free(records); return 1; }
    printf("Φορτώθηκαν %d εγγραφές.\n", n);

    int i;
    if (choice[0] == 'A' || choice[0] == 'a') {
        AVLNode *root = NULL;
        printf("Δημιουργία AVL BST (κατά Date)...\n");
        for (i = 0; i < n; i++)
            root = avlInsertDate(root, records[i].date, records[i].cumulative);
        printf("Έτοιμο.\n");
        menuAVLDate(&root);

    } else if (choice[0] == 'B' || choice[0] == 'b') {
        AVLNode *root = NULL;
        printf("Δημιουργία AVL BST (κατά Cumulative)...\n");
        for (i = 0; i < n; i++)
            root = avlInsertCumul(root, records[i].date, records[i].cumulative);
        printf("Έτοιμο.\n");
        menuAVLCumul(root);

    } else if (choice[0] == 'C' || choice[0] == 'c') {
        HashTable *ht = hashCreate(HASH_M);
        printf("Δημιουργία Hash Table (m=%d)...\n", HASH_M);
        for (i = 0; i < n; i++)
            hashInsert(ht, records[i].date, records[i].cumulative);
        printf("Έτοιμο.\n");
        menuHash(ht);

    } else {
        printf("Άκυρη επιλογή.\n");
    }

    free(records);
    return 0;
}
