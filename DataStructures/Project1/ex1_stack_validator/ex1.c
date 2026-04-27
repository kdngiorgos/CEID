#include <stdio.h>
#include <string.h>

#define MAX 100

/* Stack me pinaka */
char stack[MAX];
int top = -1;

void push(char c) {
    stack[++top] = c;
}

char pop(void) {
    return stack[top--];
}

int is_empty(void) {
    return top == -1;
}

/* Epistrefei 1 an i ekfrasi exei sosta tis parentheseis */
int check_expression(const char *expr) {
    char c;

    top = -1; /* reset stack */

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
