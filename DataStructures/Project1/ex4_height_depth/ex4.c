/*
 * Άσκηση 4 - Ύψος Δέντρου και Βάθος Κόμβων
 *
 * height():      υπολογίζει το ύψος του δέντρου.
 * printDepths(): εκτυπώνει το βάθος κάθε κόμβου.
 *
 * Ορισμός ύψους εδώ: δέντρο με μόνο ρίζα έχει ύψος 1,
 * κενό δέντρο έχει ύψος 0.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
} Node;

Node* newNode(int key) {
    Node *n = (Node*)malloc(sizeof(Node));
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

void insert(Node **root, int key) {
    if (*root == NULL) { *root = newNode(key); return; }
    if (key < (*root)->key) insert(&(*root)->left, key);
    else if (key > (*root)->key) insert(&(*root)->right, key);
}

/* Ύψος δέντρου */
int height(Node *root) {
    if (root == NULL) return 0;
    int lh = height(root->left);
    int rh = height(root->right);
    return 1 + (lh > rh ? lh : rh); // max(lh, rh) + 1
}

/* Εκτύπωση βάθους κάθε κόμβου (depth = απόσταση από ρίζα) */
void printDepths(Node *root, int depth) {
    if (root == NULL) return;
    printf("  Node %-4d -> depth %d\n", root->key, depth);
    printDepths(root->left,  depth + 1);
    printDepths(root->right, depth + 1);
}

int main() {
    Node *root = NULL;

    /*
     * Δέντρο:
     *              50
     *            /    \
     *          30      70
     *         /  \    /  \
     *        20  40  60   80
     *       /
     *      10
     */
    int keys[] = {50, 30, 70, 20, 40, 60, 80, 10};
    int i;
    int n = 8;
    for (i = 0; i < n; i++)
        insert(&root, keys[i]);

    printf("Height: %d\n\n", height(root));

    printf("Depths:\n");
    printf("-------------------\n");
    printDepths(root, 0);

    return 0;
}
