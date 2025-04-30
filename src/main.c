#include "lexer.h"

static void FreeAll ( void );

int main ( void ) {
    Lexer("/home/angel/CLionProjects/C-compiler-project/src/program.txt");
    FreeAll();
    return 0;
}

void FreeAll ( void ) {
    CloseLexerResult();
}