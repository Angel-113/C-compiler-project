#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"

const Token integer_type = INT;
const Token float_type = FLOAT;

static Token currentToken;
static TypedValue tokenValue;

static TypedValue getLexemeValue ( Symbol sym ) {
    TypedValue val;
    if ( sym->token == INTCONST) {
        val.type = integer_type;
        val.values.float_value = atoi(sym->lexeme);
    }
    else {
        val.type = float_type;
        val.values.float_value = atof(sym->lexeme);
    }
    return val;
}

// Advance lexer
void advance() {
    Symbol lexeme = lexerNextTok();
    currentToken = lexeme->token;
    tokenValue = getLexemeValue(lexeme);
}

// Match and move forward
void match(Token expected) {
    if (currentToken == expected) {
        advance();
    } else {
        printf("Syntax Error: Expected %d but found %d\n", expected, currentToken);
        exit(1);
    }
}

// Recursive Descent Parsing with Synthesized Attributes

TypedValue expression() {
    TypedValue tVal = term();
    return expressionPrime(tVal);
}

TypedValue expressionPrime(TypedValue inheritedVal) {
    if (currentToken == ADDOP) {
        match(ADDOP);
        TypedValue tVal = term();
        return expressionPrime(inheritedVal + tVal);
    } else if (currentToken == SUBOP) {
        match(SUBOP);
        TypedValue tVal = term();
        return expressionPrime(inheritedVal - tVal);
    }
    return inheritedVal;
}

TypedValue term() {
    TypedValue fVal = factor();
    return termPrime(fVal);
}

TypedValue termPrime(TypedValue inheritedVal) {
    if (currentToken == MULTOP) {
        match(MULTOP);
        TypedValue fVal = factor();
        return  termPrime(inheritedVal.values * fVal);
    } else if (currentToken == DIVOP) {
        match(DIVOP);
        TypedValue fVal = factor();
        return termPrime(inheritedVal / fVal);
    }
    return inheritedVal;
}

TypedValue factor() {
    if (currentToken == INTCONST) {
        TypedValue val;
        val.type = integer_type;
        val.values.integer_value = tokenValue.values.integer_value;
        match(INTCONST);
        return val;
    }
    else if ( currentToken == FLOATCONST ) {
        TypedValue val;
        val.type = float_type;
        val.values.float_value = tokenValue.values.float_value;
        return val;
    }
    else if (currentToken == LPAREN) {
        match(LPAREN);
        TypedValue val = expression();
        match(RPAREN);
        return val;
    } else {
        printf("Syntax Error: Invalid factor\n");
        exit(1);
    }
}

__attribute__((unused)) TypedValue performOperation(TypedValue left, TypedValue right, Token op) {
    TypedValue result;
    // Determine result type (float if either operand is float)
    result.type = (left.type == float_type || right.type == float_type) ? float_type : integer_type;

    // Perform operation based on type
    switch (op) {
        case ADDOP:
            if (result.type == integer_type) {
                result.values.integer_value = left.values.integer_value + right.values.integer_value;
            } else {
                // Convert to float if needed
                float leftVal = (left.type == integer_type) ? (float)left.values.integer_value : left.values.float_value;
                float rightVal = (right.type == integer_type) ? (float)right.values.integer_value: right.values.float_value;
                result.values.float_value = leftVal + rightVal;
            }
            break;
        case MULTOP:
            if ( result.type == integer_type ) {
                result.values.integer_value = left.values.integer_value * right.values.integer_value;
            } else {
                float leftVal = (left.type == integer_type) ? (float)left.values.integer_value : left.values.float_value;
                float rightVal = (right.type == integer_type) ? (float)right.values.integer_value : right.values.float_value;
                result.values.float_value = leftVal * rightVal;
            }
            break;
        case DIVOP:
            if ( result.type == integer_type ) {
                result.values.integer_value = left.values.integer_value / right.values.integer_value;
            } else {
                float leftVal = (left.type == integer_type) ? (float)left.values.integer_value : left.values.float_value;
                float rightVal = (right.type == integer_type) ? (float)right.values.integer_value : right.values.float_value;
                result.values.float_value = leftVal / rightVal;
            }
            break;
        case SUBOP:
            if ( result.type == integer_type ) {
                result.values.integer_value = left.values.integer_value - right.values.integer_value;
            } else {
                float leftVal = (left.type == integer_type) ? (float)left.values.integer_value : left.values.float_value;
                float rightVal = (right.type == integer_type) ? (float)right.values.integer_value : right.values.float_value;
                result.values.float_value = leftVal - rightVal;
            }
            break;
    }

    return result;
}

__attribute__((unused)) void parseWhileLoop() {
    match(WHILE);
    match(LPAREN);
    TypedValue condition = expression();
    match(RPAREN);
    if (condition.type != integer_type) {
        printf("Semantic Error: Loop condition must be an integer expression\n");
        exit(1);
    }
    /*parse init goes here*/
}

__attribute__((unused)) void parseForLoop() {
    match(FOR);
    match(LPAREN);

    /*parse init goes here*/
    match(SEMICOLON);
    TypedValue condition = expression();
    match(SEMICOLON);
    if (condition.type != integer_type) {
        printf("Semantic Error: Loop condition must be an integer expression\n");
        exit(1);
    }
    /*parse update goes here*/
    match(RPAREN);
    /*last parsing goes here*/
}