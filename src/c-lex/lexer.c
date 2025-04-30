#include "lexer.h"

/* Principal Linkage Functions */
static void InitLexer ( const char* path );
static void MainLexer ( void );
static void CloseLexer ( void );
static void PrintTokens ( void );

/* Helpers */
static void AddToken ( char* lexeme, unsigned int line, unsigned int size, TokenType type );
static void Preprocessing (void );
static bool IsAlpha ( char c );
static bool IsDigit ( char c );
static void Identifiers ( void );
static void String(void);
static void Number(void);
static void Character(void);
static void Keywords ( void );

Scanner scanner = { 0 };
LAResult result = { 0 };

static char tBuffer[100] = { 0 }; /* Token buffer for lexeme */
static unsigned int current = 0;

void Lexer (const char* path ) {
    InitLexer(path);
    MainLexer();
    CloseLexer();
}

void MainLexer( void ) {

    while ( scanner.peek() != '\0' ) {

        Preprocessing();

        tBuffer[current] = scanner.peek();

        /* In here goes the main scanner-lexer */

        switch ( tBuffer[current] ) {

            default:

                if ( IsDigit(tBuffer[current]) ) {
                    /* function to manage number literals */
                    Number();
                }
                else if ( IsAlpha(tBuffer[current]) ) {
                    /* function to manage identifiers */
                    Identifiers();
                }
                else {
                    sprintf(stderr, "Unexpected character at line %d : %s", scanner.get_line_number(), tBuffer);
                    exit(EXIT_FAILURE);
                }

                break;

            /* Single-character Tokens */
            case '(': AddToken(tBuffer, scanner.get_line_number(), current + 1, LEFT_PAREN); break;
            case ')': AddToken(tBuffer, scanner.get_line_number(), current + 1, RIGHT_PAREN); break;
            case '{': AddToken(tBuffer, scanner.get_line_number(), current + 1, LEFT_BRACE); break;
            case '}': AddToken(tBuffer, scanner.get_line_number(), current + 1, RIGHT_BRACE); break;
            case ',': AddToken(tBuffer, scanner.get_line_number(), current + 1, COMMA); break;
            case '.': AddToken(tBuffer, scanner.get_line_number(), current + 1, DOT); break;

            /* Two-character Tokens */
            case '-':
                if ( scanner.match('=') )
                    AddToken(tBuffer, scanner.get_line_number(), current + 2, MINUS_EQUAL);
                else
                    AddToken(tBuffer, scanner.get_line_number(), current + 1, MINUS);
                break;

            case '+':
                if ( scanner.match('=') )
                    AddToken(tBuffer, scanner.get_line_number(), current + 2, PLUS_EQUAL);
                else
                    AddToken(tBuffer, scanner.get_line_number(), current + 1, PLUS);
                break;

            case ';': AddToken(tBuffer, scanner.get_line_number(), current + 1, SEMICOLON); break;

            case '/':
                if ( scanner.match('=') )
                    AddToken(tBuffer, scanner.get_line_number(), current + 2, SLASH_EQUAL);
                else
                    AddToken(tBuffer, scanner.get_line_number(), current + 1, SLASH);
                break;

            case '*':
                if ( scanner.match('=') )
                    AddToken(tBuffer, scanner.get_line_number(), current + 2, STAR_EQUAL);
                else
                    AddToken(tBuffer, scanner.get_line_number(), current + 1, STAR);
                break;

            case '&':
                AddToken(tBuffer, scanner.get_line_number(), current + 1, BIT_AND);
                break;

            case '|':
                AddToken(tBuffer, scanner.get_line_number(), current + 1, BIT_OR);
                break;

            case '^':
                AddToken(tBuffer, scanner.get_line_number(), current + 1, X_OR);
                break;

            case '!': AddToken(tBuffer, scanner.get_line_number(), current + 1, NOT); break;

            case '"':
                /* Function to manage string literals  */
                String();
                break;

            case '\'':
                /* Function to manage character literals */
                Character();
                break;

        }

        current++;
        scanner.advance();

    }

    PrintTokens();

}

void InitLexer ( const char* path ) {
    __TokenList = malloc( __CurrentSize * sizeof(Token) );
    scanner = InitScanner(path);
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
    for (int i = 0; i < __Tokens; i++ ) printToken(__TokenList[i]);
}

void AddToken ( char* lexeme, unsigned int line, unsigned int size, TokenType type ) {
    addToken( makeToken( lexeme, line, size, type ) );
    memset( &tBuffer[0], 0, size );
    current = 0;
}

void Preprocessing (void ) { /* By this moment it is only required to ignore comments and preprocessor instructions */
    char c = scanner.peek();
    if ( c == ' ' || c == '\n' || c == '\t' ) scanner.advance();
    else if ( c == '/' ) {
        int b = scanner.match('*') ? '\\' : (scanner.match('/') ? '\n' : '\0');
        if ( b == '\0' ) return;
        while ( scanner.peek() != b ) scanner.advance();
        scanner.advance();
    }
    else if ( c == '#' ) while ( scanner.peek() != '\n' ) scanner.advance();
}

bool IsAlpha ( char c ) {
    return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_');
}

bool IsDigit ( char c ) {
    return '0' <= c && c <= '9';
}