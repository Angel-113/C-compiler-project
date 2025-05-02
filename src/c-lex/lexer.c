#include "lexer.h"

/* Principal Linkage Functions */
static void InitLexer ( const char* path );
static void MainLexer ( void );
static void CloseLexer ( void );
static void PrintTokens ( void );

/* Helpers */
static void AddToken ( TokenType type );
static bool IsAlpha ( char c );
static bool IsDigit ( char c );
static void Identifiers ( void );
static void String ( void );
static void Number ( void );
static void Character ( void );
static void Comments ( void );
static void Preprocessor ( void );

Scanner scanner = { 0 }; /* Scanner for reading the file */
LAResult result = { 0 }; /* Where to save the Lexical Analysis results */
Map map = { 0 }; /* A hash table for identifying keywords */

static char tBuffer[100] = { 0 }; /* Buffer for lexeme */
static unsigned int current = 0; /* Current buffer position */

void Lexer ( const char* path ) {
    InitLexer(path);
    MainLexer();
    CloseLexer();
}

void MainLexer( void ) { /* Lexer structure is based on Crafting Interpreters: https://craftinginterpreters.com/scanning.html */

    while ( scanner.peek() != '\0' ) {

        switch ( scanner.peek() ) {

            default:

                if ( IsDigit(scanner.peek()) ) {
                    /* function to manage number literals */
                    Number();
                }
                else if ( IsAlpha(scanner.peek()) ) {
                    /* function to manage identifiers */
                    Identifiers();
                }
                else {
                    sprintf( stderr, "Unexpected character at line %d : %s", scanner.get_line_number(), tBuffer );
                    exit(EXIT_FAILURE);
                }

                break;

            /* Single-character Tokens */
            case '(': AddToken(LEFT_PAREN); break;
            case ')': AddToken(RIGHT_PAREN); break;
            case '{': AddToken(LEFT_BRACE); break;
            case '}': AddToken(RIGHT_BRACE); break;
            case ',': AddToken(COMMA); break;
            case '.': AddToken(DOT); break;

            /* Two-character Tokens */
            case '=':
                if ( scanner.match('=') )
                    AddToken(EQUAL_EQUAL);
                else
                    AddToken(EQUAL);
                break;

            case '-':
                if ( scanner.match('=') )
                    AddToken(MINUS_EQUAL);
                else
                    AddToken(MINUS);
                break;

            case '+':
                if ( scanner.match('=') )
                    AddToken(PLUS_EQUAL);
                else
                    AddToken(PLUS);
                break;

            case ';': AddToken(SEMICOLON); break;

            case '/':
                if ( scanner.match('=') )
                    AddToken(SLASH_EQUAL);
                else if ( scanner.match('*') || scanner.match('/'))
                    Comments();
                else
                    AddToken(SLASH);
                break;

            case '*':
                if ( scanner.match('=') )
                    AddToken(STAR_EQUAL);
                else
                    AddToken(STAR);
                break;

            case '&':
                AddToken(BIT_AND);
                break;

            case '|':
                AddToken(BIT_OR);
                break;

            case '^':
                AddToken(X_OR);
                break;

            case '!': AddToken(NOT); break;

            case '"':
                /* Function to manage string literals  */
                String();
                break;

            case '\'':
                /* Function to manage character literals */
                Character();
                break;

            case '#':
                Preprocessor();
                break;

            case '\n':
            case '\t':
            case ' ':
                current = 0;
                break;
        }

        tBuffer[current] = scanner.peek();
        current++;
        scanner.advance();

    }

    PrintTokens();

}

void InitLexer ( const char* path ) {
    __TokenList = malloc( __CurrentSize * sizeof(Token) );
    scanner = InitScanner(path);
    map = InitMap();
}

void CloseLexer ( void ) {
    result.Tokens = malloc( __Tokens * sizeof(Token) );
    memcpy( result.Tokens, __TokenList, __Tokens * sizeof(Token) );
    result.tTokens = __Tokens;
    freeTokenList();
    scanner.destroy();
}

void CloseLexerResult (void ) {
    if (result.Tokens) free(result.Tokens);
    result.Tokens = NULL;
}

void PrintTokens ( void ) {
    if ( !__TokenList ) return;
    for ( int i = 0; i < __Tokens; i++ ) printToken(__TokenList[i]);
}

void AddToken ( TokenType type ) {
    addToken( makeToken( tBuffer, scanner.get_line_number(), current, type ) );
    memset( &tBuffer[0], 0, current ); /* Is it really necessary to set all the chars in tBuffer to 0 when adding a new token? */
    current = 0;
}

bool IsAlpha ( char c ) {
    return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_');
}

bool IsDigit ( char c ) {
    return '0' <= c && c <= '9';
}

void Number ( void ) {

    while ( IsDigit( scanner.peek() ) ) {
        current++;
        scanner.advance();
        tBuffer[current] = scanner.peek();
    }

    if ( scanner.peek() == '.' && IsDigit(scanner.next()) ) {
        scanner.advance();
        while ( IsDigit(scanner.peek()) ) {
            current++;
            tBuffer[current] = scanner.peek();
            scanner.advance();
        }
    }

    AddToken(NUMBER);

}

void Identifiers ( void ) {
    while ( IsDigit( scanner.peek() ) || IsAlpha( scanner.peek() ) ) {
        tBuffer[current] = scanner.peek();
        scanner.advance();
        current++;
    }
    TokenType type = map.get_token( tBuffer, current );
    AddToken( type != ERROR ? type : IDENTIFIER );
}

void String ( void ) {
    scanner.advance();
    while ( scanner.peek() != '"' ) {
        if ( scanner.peek() == '\0' ) {
            sprintf( (char*)stderr, "Unterminated string literal\n" );
            exit(EXIT_FAILURE);
        }
        scanner.advance();
        tBuffer[++current] = scanner.peek();
    }
    tBuffer[++current] = scanner.peek();
    AddToken(STRING);
}

void Character ( void ) {
    unsigned int start = current;
    scanner.advance();
    while ( current - start < 3 ) {
        tBuffer[current] = scanner.peek();
        current++;
    }
    if ( scanner.peek() != '\'' ) {
        sprintf( (char*)stderr, "Unterminated char literal\n");
        exit(EXIT_FAILURE);
    }
    AddToken(CHARACTER);
}

void Comments ( void ) { /* Ignore comments */
    int end = scanner.peek() == '*' ? '/' : scanner.peek() == '/' ? '\n' : '\0';
    while ( scanner.peek() != end && end != '\0' ) scanner.advance();
}

void Preprocessor ( void ) { /* At this moment it is only required to ignore the preprocessor instructions */
    while ( scanner.peek() != '\n' ) scanner.advance();
}