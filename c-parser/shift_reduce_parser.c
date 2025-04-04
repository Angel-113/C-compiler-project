#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

typedef struct {
    Token token;
    int value;
} StackItem;

StackItem stack[100];
int top = -1;

void push(Token token, int value) {
    stack[++top] = (StackItem){token, value};
}

StackItem pop() {
    return stack[top--];
}

static int getLexemeValue ( Symbol sym ) {
    if ( sym->token == INTCONST) return atoi(sym->lexeme);
}

void shift(Token token, int value) {
    push(token, value);
}

void reduce() {
    StackItem right = pop();
    StackItem op = pop();
    StackItem left = pop();

    int result;
    if (op.token == ADDOP) result = left.value + right.value;
    else if (op.token == SUBOP) result = left.value - right.value;
    else if (op.token == MULTOP) result = left.value * right.value;
    else if (op.token == DIVOP) result = left.value / right.value;

    push(INTCONST, result);
}

int parse() {
    Token token;
    int value;

    while ((token = lexerNextTok()->token) != NULL) {
        value = getLexemeValue();

        if (token == INTCONST) {
            shift(token, value);
        } else if (token == ADDOP || token == SUBOP || token == MULTOP || token == DIVOP) {
            while (top >= 2 && stack[top - 1].token != LPAREN) {
                reduce();
            }
            shift(token, 0);
        } else if (token == LPAREN) {
            shift(token, 0);
        } else if (token == RPAREN) {
            while (stack[top - 1].token != LPAREN) {
                reduce();
            }
            pop(); // Remove '('
        }
    }

    while (top > 0) {
        reduce();
    }

    return stack[top].value;
}

