#include "lexer.h"
#include <stdio.h>
#include "Symbol.h"
#include "parser.h"


void init( void )
{
    regexInit();
    symtableInit();
    lexerInit();
}


void destroy()
{
    symtableDestroy();
    lexerDestroy();
    regexDestroy();
}


int main( int argc, char *argv[] )
{

    if( argc < 2 )
    {
        printf(" Usage: main <file>\n Parse with attributes a file\n" );
        return 1;
    }

    init();

/*
 * Comente este código porque esta es el uso de la implementación del lexer:
 *

    if( lexerOpenFile( argv[1] ) )
    {
        Symbol sym;
        sym = lexerNextTok();
        while( sym != NULL )
        {
            printf( "Symbol found! Token: %s -- Lexeme: %s\n", tokNames[sym->token], sym->lexeme );
            sym = lexerNextTok();
        }
        lexerCloseFile();
    }
    else
    {
        printf( "Couldn't open file: %s\n", argv[1] );
    }

    destroy();

*/

    if (lexerOpenFile(argv[1])) {
        advance();
        printf("%d", expression().values.integer_value);
    }

    destroy();

    return 0;
}