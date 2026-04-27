#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Komvos listas gia ti stoiva */
typedef struct Node {
    char data;
    struct Node *next;
} Node;

Node *top = NULL;

void push(char c) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = c;
    new_node->next = top;
    top = new_node;
}

char pop(void) {
    Node *temp = top;
    char c = temp->data;
    top = top->next;
    free(temp);
    return c;
}

int is_empty(void) {
    return top == NULL;
}

/* Apeleftherwsi olwn twn komvwn */
void clear_stack(void) {
    while (!is_empty())
        pop();
}

/* Epistrefei 1 an i ekfrasi exei sosta tis parentheseis */
int check_expression(const char *expr) {
    char c;

    clear_stack(); /* reset stack */

    for (int i = 0; expr[i] != '\0'; i++) {
        c = expr[i];

        if (c == '(' || c == '[' || c == '{') {
            push(c);
        }
        else if (c == ')' || c == ']' || c == '}') {
            if (is_empty())
                return 0;

            char open = pop();

            if (c == ')' && open != '(') return 0;
            if (c == ']' && open != '[') return 0;
            if (c == '}' && open != '{') return 0;
        }
    }
    return is_empty(); /* 1 an i stoiva einai adeia, 0 an oxi */
}

int main(void) {
    const char *tests[] = {
        "{[()]}",
        "((()))",
        "{[}]",
        "(()",
        "()[]{}",
        "",
        "({[a+b]*c})",
        "([)]"
    };

    int n = sizeof(tests) / sizeof(tests[0]);
    int i;

    for (i = 0; i < n; i++) {
        int result = check_expression(tests[i]);
        printf("%-20s -> %s\n", tests[i], result ? "Sosti" : "Lathos");
    }

    return 0;
}
