/*
 * Άσκηση 2 - Δυαδικό Δέντρο Αναζήτησης (BST)
 *
 * Node-oriented BST.
 * Υλοποιεί: αναζήτηση, εισαγωγή, διαγραφή.
 * Η διαγραφή καλύπτει και τις 3 περιπτώσεις:
 *   1) Φύλλο
 *   2) Κόμβος με ένα παιδί
 *   3) Κόμβος με δύο παιδιά (αντικατάσταση με τον inorder successor)
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
} Node;

/* --- Βοηθητικές συναρτήσεις --- */

Node* newNode(int key) {
    Node *n = (Node*)malloc(sizeof(Node));
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

/* Αναζήτηση: επιστρέφει pointer στον κόμβο ή NULL */
Node* binaryTreeSearch(Node *root, int key) {
    if (root == NULL || root->key == key)
        return root;
    if (key < root->key)
        return binaryTreeSearch(root->left, key);
    return binaryTreeSearch(root->right, key);
}

/* Εισαγωγή */
void insert(Node **root, int key) {
    if (*root == NULL) {
        *root = newNode(key);
        return;
    }
    if (key < (*root)->key)
        insert(&(*root)->left, key);
    else if (key > (*root)->key)
        insert(&(*root)->right, key);
    /* αν key == root->key, αγνοούμε */
}

/* Βρες τον κόμβο με το ελάχιστο κλειδί (inorder successor) */
Node* minNode(Node *root) {
    while (root->left != NULL)
        root = root->left;
    return root;
}

/* Διαγραφή */
Node* deleteNode(Node *root, int key) {
    if (root == NULL) return NULL;

    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    } else {
        /* Βρέθηκε ο κόμβος */
        if (root->left == NULL) {
            /* Περίπτωση 1 & 2: φύλλο ή μόνο δεξί παιδί */
            Node *tmp = root->right;
            free(root);
            return tmp;
        } else if (root->right == NULL) {
            /* Περίπτωση 2: μόνο αριστερό παιδί */
            Node *tmp = root->left;
            free(root);
            return tmp;
        }
        /* Περίπτωση 3: δύο παιδιά */
        /* Βρίσκουμε τον inorder successor (min του δεξιού υποδέντρου) */
        Node *successor = minNode(root->right);
        root->key = successor->key;
        root->right = deleteNode(root->right, successor->key);
    }
    return root;
}

/* Inorder για εκτύπωση (επαλήθευση) */
void inorder(Node *root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
}

int main() {
    Node *root = NULL;

    /* Εισαγωγή κόμβων */
    int keys[] = {50, 30, 70, 20, 40, 60, 80, 10, 35};
    int i;
    int n = 9;
    for (i = 0; i < n; i++)
        insert(&root, keys[i]);

    printf("Inorder : ");
    inorder(root);
    printf("\n\n");

    /* Αναζήτηση */
    int searchKeys[] = {40, 45, 10};
    for (i = 0; i < 3; i++) {
        Node *found = binaryTreeSearch(root, searchKeys[i]);
        printf("Search %d: %s\n", searchKeys[i], found ? "Found" : "Not Found");
    }
    printf("\n");

    /* Διαγραφή φύλλου */
    root = deleteNode(root, 10);
    printf("Delete 10:     ");
    inorder(root);
    printf("\n");

    /* Διαγραφή κόμβου με ένα παιδί */
    root = deleteNode(root, 20);
    printf("Delete 20 (1 child):  ");
    inorder(root);
    printf("\n");

    /* Διαγραφή κόμβου με δύο παιδιά */
    root = deleteNode(root, 30);
    printf("Delete 30 (2 children): ");
    inorder(root);
    printf("\n");

    return 0;
}
