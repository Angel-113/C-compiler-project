#ifndef GRAMMAR_KEYWORDS_H
#define GRAMMAR_KEYWORDS_H

#include "base.h"
#include "token.h"

typedef struct Bucket {
    char lBuffer[100];
    TokenType type;
} Bucket;

typedef struct Map {
    TokenType (*get_token) ( char*, unsigned int);
} Map;

extern Map InitMap ( void );

#endif //GRAMMAR_KEYWORDS_H
