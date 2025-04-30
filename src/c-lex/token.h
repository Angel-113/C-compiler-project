#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-reserved-identifier"
#ifndef GRAMMAR_TOKEN_H
#define GRAMMAR_TOKEN_H

#include "base.h"

typedef enum {
    /* One-character tokens : Tokens de un caracter */
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR, BIT_AND,
    BIT_OR, X_OR, NOT,

    /* Two-character tokens : Tokens de dos caracteres */
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESSER, LESSER_EQUAL,
    PLUS_EQUAL, MINUS_EQUAL,
    SLASH_EQUAL, STAR_EQUAL,
    AND_AND, OR_OR,

    /* Literals : Literal (valores) */
    IDENTIFIER, STRING, CHARACTER, NUMBER,

    /* Keywords : Palabras Clave */
    AUTO, BREAK, CASE, CHAR, CONST,
    CONTINUE, DEFAULT, DO, DOUBLE, ELSE,
    ENUM, EXTERN, FLOAT, FOR, GOTO,
    IF, INLINE, INT, LONG, REGISTER,
    RESTRICT, RETURN, SHORT, SIGNED, SIZEOF,
    STATIC, STRUCT, SWITCH, TYPEDEF, UNION,
    UNSIGNED, VOID, VOLATILE, WHILE,

    /* Special tokens : Tokens especiales */
    ERROR, END
} TokenType;

extern const char* TOKEN_TYPE_STRINGS[];

typedef struct Token {
    TokenType type;
    unsigned int line;
    unsigned int size;
    char lexeme[100];
} Token;

extern Token* __TokenList; /* List of Tokens */
extern unsigned int __Tokens; /* current amount of tokens */
extern unsigned int __CurrentSize; /* Initialization size for TokenList */

extern void printToken ( Token t );
extern Token makeToken ( char* lexeme, unsigned int line, unsigned int size, TokenType type );
extern void addToken ( Token t );
extern void freeTokenList ( void );

#endif //GRAMMAR_TOKEN_H

#pragma clang diagnostic pop