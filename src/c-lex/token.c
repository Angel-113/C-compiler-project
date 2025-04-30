#include "token.h"

const char* TOKEN_TYPE_STRINGS[] = {
        /* One-character tokens */
        "(", ")", "{", "}", ",", ".", "-", "+", ";", "/", "*", "&",
        "|", "^", "!",

        /* Two-character tokens */
        "=", "==",
        ">", ">=",
        "<", "<=",
        "+=", "-=",
        "/=", "*=",
        "&&", "||",

        /* Literals */
        "IDENTIFIER", "STRING", "CHARACTER","NUMBER",

        /* Keywords */
        "auto", "break", "case", "char", "const",
        "continue", "default", "do", "double", "else",
        "enum", "extern", "float", "for", "goto",
        "if", "inline", "int", "long", "register",
        "restrict", "return", "short", "signed", "sizeof",
        "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while",

        /* Special tokens */
        "ERROR", "END"
};

unsigned int __CurrentSize = 100;
unsigned int __Tokens = 0;
Token* __TokenList = NULL;

void printToken ( Token t ) {
    printf( "{ %s | %s } \n", TOKEN_TYPE_STRINGS[t.type], t.lexeme );
}

Token makeToken ( char* lexeme, unsigned int line, unsigned size, TokenType type ) {
    Token t = (Token) {
            type,
            line,
            size,
    };
    memcpy( &t.lexeme, lexeme, size );
    return t;
}

void addToken ( Token t ) {

    if ( !__TokenList ) {
        sprintf( stderr, "__TokenList not initialized\n" );
        exit(EXIT_FAILURE);
    }

    if ( __CurrentSize <= __Tokens ) {
        void* aux = realloc( __TokenList, 2 * __CurrentSize * sizeof(Token));
        if ( !aux ) {
            sprintf( stderr, "Not enough memory to realloc __TokenList\n" );
            exit(EXIT_FAILURE);
        }
        else {
            __TokenList = aux;
            __CurrentSize *= 2;
        }
    }

    __TokenList[__Tokens++] = t;

}

void freeTokenList ( void ) {
    if ( __TokenList ) free(__TokenList);
}