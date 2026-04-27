/*
 * Άσκηση 3ii - Προδιάταξη (Preorder) και Μεταδιάταξη (Postorder)
 *
 * Preorder:  root -> αριστερό υποδέντρο -> δεξί υποδέντρο
 * Postorder: αριστερό υποδέντρο -> δεξί υποδέντρο -> root
 * Inorder:   αριστερό υποδέντρο -> root -> δεξί υποδέντρο 
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

void inorder(Node *root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
}

void preorder(Node *root) {
    if (root == NULL) return;
    printf("%d ", root->key);
    preorder(root->left);
    preorder(root->right);
}

void postorder(Node *root) {
    if (root == NULL) return;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->key);
}

int main() {
    Node *root = NULL;

    /*
     * Δέντρο που δημιουργούμε:
     *
     *           50
     *          /  \
     *        30    70
     *       /  \  /  \
     *      20  40 60  80
     */
    int keys[] = {50, 30, 70, 20, 40, 60, 80};
    int i;
    int n = 7;
    for (i = 0; i < n; i++)
        insert(&root, keys[i]);

    printf("Inorder:   ");
    inorder(root);
    printf("\n");

    printf("Preorder:  ");
    preorder(root);
    printf("\n");

    printf("Postorder: ");
    postorder(root);
    printf("\n");

    return 0;
}
