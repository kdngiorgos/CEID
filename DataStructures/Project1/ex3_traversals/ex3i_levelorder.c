/*
 * Άσκηση 3i - Level-Order Traversal
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE 200

/* --- Δέντρο BST --- */

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

typedef struct {
    Node *data[MAX_QUEUE];
    int front;
    int rear;
    int size;
} Queue;

void initQueue(Queue *q) {
    q->front = q->rear = q->size = 0;
}

int isEmptyQ(Queue *q) {
    return q->size == 0;
}

void enqueue(Queue *q, Node *n) {
    if (q->size < MAX_QUEUE) {
        q->data[q->rear] = n;
        q->rear = (q->rear + 1) % MAX_QUEUE;
        q->size++;
    }
}

Node* dequeue(Queue *q) {
    if (!isEmptyQ(q)) {
        Node *n = q->data[q->front];
        q->front = (q->front + 1) % MAX_QUEUE;
        q->size--;
        return n;
    }
    return NULL;
}

/* --- Level-Order Traversal --- */

void levelOrder(Node *root) {
    if (root == NULL) return;

    Queue q;
    initQueue(&q);
    enqueue(&q, root);

    int level = 0;
    int nodesInLevel = 1;       /* κόμβοι στο τρέχον επίπεδο */
    int nodesInNext = 0;        /* κόμβοι στο επόμενο επίπεδο */

    printf("Level %d: ", level);

    while (!isEmptyQ(&q)) {
        Node *curr = dequeue(&q);
        printf("%d ", curr->key);
        nodesInLevel--;

        if (curr->left)  { enqueue(&q, curr->left);  nodesInNext++; }
        if (curr->right) { enqueue(&q, curr->right); nodesInNext++; }

        if (nodesInLevel == 0 && !isEmptyQ(&q)) {
            level++;
            nodesInLevel = nodesInNext;
            nodesInNext = 0;
            printf("\nLevel %d: ", level);
        }
    }
    printf("\n");
}

int main() {
    Node *root = NULL;
    int keys[] = {50, 30, 70, 20, 40, 60, 80, 10, 35, 45};
    int i;
    int n = 10;
    for (i = 0; i < n; i++)
        insert(&root, keys[i]);

    printf("Level-order traversal:\n");
    printf("--------------------------------------\n");
    levelOrder(root);

    return 0;
}
