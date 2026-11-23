#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 50
#define MAX_LINE 1024

typedef struct {
    double data[MAX_SIZE];
    int top;
} Stack;

void init(Stack *s) {
    s->top = -1;
}

int is_empty(Stack *s) {
    return s->top == -1;
}

int is_full(Stack *s) {
    return s->top == MAX_SIZE - 1;
}

int push(Stack *s, double value) {
    if (is_full(s)) {
        return 0;
    }

    s->top++;
    s->data[s->top] = value;

    return 1;
}

int pop(Stack *s, double *value) {
    if (is_empty(s)) {
        return 0;
    }

    *value = s->data[s->top];
    s->top--;

    return 1;
}

int peek(Stack *s, double *value) {
    if (is_empty(s)) {
        return 0;
    }

    *value = s->data[s->top];
    return 1;
}

int main() {
    Stack s;
    init(&s);

    char line[MAX_LINE];

    printf("Enter postfix expression: ");

    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error: no input.\n");
        return 1;
    }

    char *token = strtok(line, " \t\r\n");

    while (token != NULL) {

        if (strcmp(token, "neg") == 0) {
            double a;

            if (!pop(&s, &a)) {
                printf("Error: not enough operands.\n");
                return 1;
            }

            if (!push(&s, -a)) {
                printf("Error: stack is full.\n");
                return 1;
            }

        } else if (strcmp(token, "+") == 0 ||
                   strcmp(token, "-") == 0 ||
                   strcmp(token, "*") == 0 ||
                   strcmp(token, "/") == 0) {

            double a, b, result;

            if (!pop(&s, &b) || !pop(&s, &a)) {
                printf("Error: not enough operands.\n");
                return 1;
            }

            if (strcmp(token, "+") == 0) {
                result = a + b;
            } else if (strcmp(token, "-") == 0) {
                result = a - b;
            } else if (strcmp(token, "*") == 0) {
                result = a * b;
            } else {
                if (b == 0) {
                    printf("Error: division by zero.\n");
                    return 1;
                }

                result = a / b;
            }

            if (!push(&s, result)) {
                printf("Error: stack is full.\n");
                return 1;
            }

        } else {
            char *end;
            double number = strtod(token, &end);

            if (end == token || *end != '\0') {
                printf("Error: invalid token %s.\n", token);
                return 1;
            }

            if (!push(&s, number)) {
                printf("Error: stack is full.\n");
                return 1;
            }
        }

        token = strtok(NULL, " \t\r\n");
    }

    double answer;

    if (s.top != 0 || !peek(&s, &answer)) {
        printf("Error: incorrect expression.\n");
        return 1;
    }

    printf("Result: %.7f\n", answer);

    return 0;
}
