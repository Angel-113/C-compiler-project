#include "token.h"

const char* TOKEN_TYPE_STRINGS[] = {
        /* One-character tokens */
        "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE", "COMA", "DOT", "MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR", "BIT_AND",
        "BIT_OR", "X_OR", "NOT", "DOTS", "LEFT_BRACKET", "RIGHT_BRACKET", "QUESTION",

        /* Two-character tokens */
        "EQUAL", "EQUAL_EQUAL",
        "GREATER", "GREATER_EQUAL",
        "LESSER", "LESSER_EQUAL",
        "PLUS_EQUAL", "MINUS_EQUAL",
        "SLASH_EQUAL", "STAR_EQUAL",
        "AND", "BIT_AND_EQUAL", "OR", "BIT_OR_EQUAL",
        "ARROW", "X_OR_EQUAL", "DIFFERENT",
        "PLUS_PLUS", "MINUS_MINUS",

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
    printf( "{ Token( %s ) | Lexeme( %s ) | Size( %d ) } \n", TOKEN_TYPE_STRINGS[t.type], t.lexeme, t.size);
}

Token makeToken ( char* lexeme, unsigned int line, unsigned size, TokenType type ) {
    Token t = (Token) {
            type,
            line,
            size == 0 ? 1 : size, /* temporary remedy for 0 sized tokens */
    };
    memcpy( &t.lexeme, lexeme, (size + 1) * sizeof(char) );
    return t;
}

void addToken ( Token t ) {

    if ( !__TokenList ) {
        fprintf( stderr, "__TokenList not initialized\n" );
        exit(EXIT_FAILURE);
    }

    if ( __CurrentSize <= __Tokens ) {
        void* aux = realloc( __TokenList, 2 * __CurrentSize * sizeof(Token));
        if ( !aux ) {
            fprintf( stderr, "Not enough memory to realloc __TokenList\n" );
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