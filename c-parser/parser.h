#ifndef GRAMMAR_PARSER_H
#define GRAMMAR_PARSER_H

#include "Symbol.h"

extern const Token integer_type;
extern const Token float_type;

typedef struct TypedValue {
    Token type;
    union {
        int integer_value;
        float float_value;
    } values;
    char* tempName;
} TypedValue;


TypedValue expression();
TypedValue expressionPrime(TypedValue InheritedValue);
TypedValue term();
TypedValue termPrime(TypedValue InheritedValue);
TypedValue factor();

__attribute__((unused)) TypedValue performOperation ( TypedValue left, TypedValue right, Token op );
void advance();

typedef enum {
    IR_LABEL,
    IR_ASSIGN,
    IR_ADD, IR_SUB, IR_MUL, IR_DIV,
    IR_CJUMP,  // Conditional jump (if condition)
    IR_JUMP,   // Unconditional jump (goto)
    IR_FUNCTION
} IROperation;

typedef struct IRInstruction {
    IROperation op;
    char *arg1;
    char *arg2;
    char *result;
    struct IRInstruction *next;
} IRInstruction;

IRInstruction *IRroot = NULL;  // Global IR list

#endif //GRAMMAR_PARSER_H
