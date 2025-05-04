//
// Created by angel on 22/04/25.
//

#ifndef GRAMMAR_LEXER_H
#define GRAMMAR_LEXER_H

#include "base.h"
#include "token.h"
#include "scanner.h"
#include "keywords.h"

#define error( str ) \
    fprintf(stderr, (str)); \
    exit(EXIT_FAILURE); \

#define error_at( str, attr1 ) \
    fprintf(stderr, (str), (attr1)); \
    exit(EXIT_FAILURE);        \

#define lexer_error( str, line, error ) \
        fprintf(stderr, (str), (line), (error)); \
        exit(EXIT_FAILURE); \


extern Scanner scanner;
extern Map map;
extern void Lexer(const char* file_path );
extern void CloseLexerResult (void); /* When 'result' no needed anymore for external programs */

typedef struct LAResult {
    unsigned int tTokens;
    Token* Tokens;
} LAResult;

extern LAResult result;

#endif //GRAMMAR_LEXER_H
