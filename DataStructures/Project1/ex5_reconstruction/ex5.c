/*
 * Άσκηση 5 - Ανακατασκευή Δέντρου από Διατάξεις
 *
 * Δίνεται η inorder και η postorder διάταξη ενός δέντρου
 * με μοναδικούς χαρακτήρες. Ανακατασκευάζουμε το δέντρο και
 * επαληθεύουμε με inorder traversal.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char key;
    struct Node *left;
    struct Node *right;
} Node;

Node* newNode(char key) {
    Node *n = (Node*)malloc(sizeof(Node));
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

void inorder(Node *root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%c ", root->key);
    inorder(root->right);
}

void preorder(Node *root) {
    if (root == NULL) return;
    printf("%c ", root->key);
    preorder(root->left);
    preorder(root->right);
}

void postorder(Node *root) {
    if (root == NULL) return;
    postorder(root->left);
    postorder(root->right);
    printf("%c ", root->key);
}

/* Βρίσκει το index ενός χαρακτήρα στο τμήμα [start, end] */
int findIndex(char *arr, int start, int end, char val) {
    int i;
    for (i = start; i <= end; i++) {
        if (arr[i] == val) return i;
    }
    return -1;
}

/*
 * Ανακατασκευή από Postorder + Inorder
 * - Το τελευταίο στοιχείο του postorder είναι πάντα η ρίζα.
 * - Βρίσκουμε τη ρίζα στο inorder -> χωρίζει το αριστερό από το δεξί υποδέντρο.
 * - Επαναλαμβάνουμε αναδρομικά για κάθε υποδέντρο.
 */
Node* reconstructFromPost(char *inord, char *postord,
                          int inStart, int inEnd,
                          int postStart, int postEnd) {
    if (inStart > inEnd || postStart > postEnd) return NULL;

    char rootVal = postord[postEnd];
    Node *root = newNode(rootVal);

    int idx = findIndex(inord, inStart, inEnd, rootVal);
    int leftSize = idx - inStart;

    root->left  = reconstructFromPost(inord, postord,
                                      inStart,    idx - 1,
                                      postStart,  postStart + leftSize - 1);
    root->right = reconstructFromPost(inord, postord,
                                      idx + 1,    inEnd,
                                      postStart + leftSize, postEnd - 1);
    return root;
}

/*
 *	Ανακατασκευή από Preorder + Inorder
 * - Το πρώτο στοιχείο του preorder είναι η ρίζα.
 * - Βρίσκουμε τη ρίζα στο inorder -> χωρίζει αριστερό/δεξί υποδέντρο.
 * - Επαναλαμβάνουμε αναδρομικά.
 */
Node* reconstructFromPre(char *inord, char *preord,
                         int inStart, int inEnd,
                         int preStart, int preEnd) {
    if (inStart > inEnd || preStart > preEnd) return NULL;

    char rootVal = preord[preStart];
    Node *root = newNode(rootVal);

    int idx = findIndex(inord, inStart, inEnd, rootVal);
    int leftSize = idx - inStart;

    root->left  = reconstructFromPre(inord, preord,
                                     inStart,     idx - 1,
                                     preStart + 1, preStart + leftSize);
    root->right = reconstructFromPre(inord, preord,
                                     idx + 1,     inEnd,
                                     preStart + leftSize + 1, preEnd);
    return root;
}

int main() {
    /*
     * Παράδειγμα:
     *
     *         A
     *        / \
     *       B   C
     *      / \ / \
     *     D  E F  G
     *
     * Inorder:   D B E A F C G
     * Postorder: D E B F G C A
     * Preorder:  A B D E C F G
     */
    char inord[]   = "DBEAFCG";
    char postord[] = "DEBFGCA";
    char preord[]  = "ABDECFG";
    int n = (int)strlen(inord);
    int i;

    printf("=== 1. Postorder + Inorder ===\n");
    printf("Inorder   input: ");
    for (i = 0; inord[i]; i++) printf("%c ", inord[i]);
    printf("\nPostorder input: ");
    for (i = 0; postord[i]; i++) printf("%c ", postord[i]);
    printf("\n\n");

    Node *root1 = reconstructFromPost(inord, postord, 0, n-1, 0, n-1);

    printf("Inorder validation:  ");
    inorder(root1);
    printf("  (same)\n");
    printf("Preorder outcome: ");
    preorder(root1);
    printf("\n\n");

    printf("=== 2. Preorder + Inorder ===\n");
    printf("Inorder  input: ");
    for (i = 0; inord[i]; i++) printf("%c ", inord[i]);
    printf("\nPreorder input: ");
    for (i = 0; preord[i]; i++) printf("%c ", preord[i]);
    printf("\n\n");

    Node *root2 = reconstructFromPre(inord, preord, 0, n-1, 0, n-1);

    printf("Inorder validation:   ");
    inorder(root2);
    printf("  (smae)\n");
    printf("Postorder outcome: ");
    postorder(root2);
    printf("\n");

    return 0;
}
