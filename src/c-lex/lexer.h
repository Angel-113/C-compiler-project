//
// Created by angel on 22/04/25.
//

#ifndef GRAMMAR_LEXER_H
#define GRAMMAR_LEXER_H

#include "base.h"
#include "token.h"
#include "Scanner.h"

extern Scanner scanner;
extern void Lexer(const char* file_path );
extern void CloseLexerResult (void ); /* When 'result' no needed anymore for external programs */

typedef struct LAResult {
    unsigned int tTokens;
    Token* Tokens;
} LAResult;

extern LAResult result;

#endif //GRAMMAR_LEXER_H
