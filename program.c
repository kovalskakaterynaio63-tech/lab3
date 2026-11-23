#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    double data;
    struct Node *next;
} Node;

typedef struct {
    Node *top;
} Stack;

int is_empty(Stack *s) {
    return s->top == NULL;
}

int is_full(Stack *s) {
    (void)s;
    return 0;
}

int push(Stack *s, double x) {
    Node *p = malloc(sizeof(Node));

    if (p == NULL) return 0;

    p->data = x;
    p->next = s->top;
    s->top = p;

    return 1;
}

int pop(Stack *s, double *x) {
    if (is_empty(s)) return 0;

    Node *p = s->top;
    *x = p->data;
    s->top = p->next;
    free(p);

    return 1;
}

int peek(Stack *s, double *x) {
    if (is_empty(s)) return 0;

    *x = s->top->data;
    return 1;
}

void clear(Stack *s) {
    double x;
    while (pop(s, &x)) {}
}

int fail(Stack *s, const char *message) {
    printf("Error: %s\n", message);
    clear(s);
    return 1;
}

int main() {
    Stack s;
    s.top = NULL;

    char line[1024];
    printf("Enter postfix expression: ");

    if (!fgets(line, sizeof(line), stdin)) {
        return fail(&s, "no input");
    }

    char *token = strtok(line, " \t\r\n");

    while (token != NULL) {
        double a, b, value;

        if (strcmp(token, "neg") == 0) {
            if (!pop(&s, &a))
                return fail(&s, "not enough numbers");

            value = -a;
        }
        else if (strlen(token) == 1 &&
                 strchr("+-*/", token[0]) != NULL) {

            if (!pop(&s, &b) || !pop(&s, &a))
                return fail(&s, "not enough numbers");

            switch (token[0]) {
                case '+': value = a + b; break;
                case '-': value = a - b; break;
                case '*': value = a * b; break;
                case '/':
                    if (b == 0)
                        return fail(&s, "division by zero");

                    value = a / b;
                    break;
            }
        }
        else {
            char *end;
            value = strtod(token, &end);

            if (end == token || *end != '\0')
                return fail(&s, "invalid expression");
        }

        if (!push(&s, value))
            return fail(&s, "not enough memory");

        token = strtok(NULL, " \t\r\n");
    }

    double result;

    if (s.top == NULL || s.top->next != NULL ||
        !peek(&s, &result)) {
        return fail(&s, "incorrect expression");
    }

    printf("Result: %.7f\n", result);

    clear(&s);
    return 0;
}
