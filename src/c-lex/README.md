# Lexical Analysis for a C-like syntax language

This section was written while reading the *Scanning* chapter from *Crafting Interpreters*:
https://craftinginterpreters.com/scanning.html. I strongly recommend to take a look on it. 

**Important note**: My english writing skills are not good enough to make documentation. 
Feel free to ask if you don't understand any of the topics in this section.

## What's a lexical analyzer?

A lexical analyzer is a *phase* of a *compilation* or *interpretation* process. 
It receives a source code file and returns an associated list of *tokens*. Those
*tokens* are every single character, combination of characters or keywords that are
going to be accepted by the language.

For example: `"int", "var_name", "1", "if", "(", "?", "!"`

There are 4 types of tokens: 

- Single character tokens: like `LEFT_PARENTHESIS : "("`.
- Two-character tokens: like increment by one `PLUS_PLUS : "++"`.
- Literals: actual values like strings, number or character literals. `NUMBER: "1", STRING: "Hello World\n", CHARACTER: '\n'`.
- Keywords: all the language keywords. `IF: "if", WHILE: "while"`.
- Identifier: every valid combination of characters that are not any of the previous ones. `IDENTIFIER: "val_name"`.

## Structs used in this compiler phase

This project is organized in a way to try to be clean and understandable. With that in mind
I decided to make it the more modular it could be. Every modulo plays a role in the lexical analysis
and does not affect (by its action) to the rest of them. 

### Scanner API

The Scanner files are made to read a file and to put it into a `char* buffer`. It defines a struct: 

```` C
typedef struct Scanner {
    char (*peek)(void);
    bool (*match)(char);
    char (*advance)(void);
    char (*next)(void);
    unsigned int (*get_line_number)(void);
    void (*destroy)(void);
} Scanner; 
````

As you could see, there are only functions into it. And every function and affected variables
are into the implementation file: `scanner.c`.

- `peek()` returns the character in the current position of the buffer.
- `match(char expected)` a conditional advance; if the current position's next character matches `expected` and it's different to `'\0'` then the `scanner` advance and return a `true`, otherwise just return `false`.
- `advance()` consume the character and returns the next to it.
- `next()` just returns the next character.
- `get_line_number()` just returns the line number the scanner is currently in.
- `destroy()` free's the dynamically allocated memory in buffer. 

```` C
static char* buffer = NULL;
static size_t buffer_size = 0;
static unsigned int pos = 0;
static unsigned int line = 1;

static char peek(void) {
    return buffer[pos];
}

static bool match(char expected) {
    if ( buffer[pos + 1] == '\0' || buffer[pos + 1] != expected ) return false;
    pos++;
    return true;
}

static char advance(void) {
    if (buffer[pos] == '\0') return '\0'; /* End of file */
    char c = buffer[++pos];
    if (c == '\n') line++;
    return c;
}

static unsigned int get_line_number(void) {
    return line;
}

static void destroy(void) {
    free(buffer);
    buffer = NULL;
    buffer_size = 0;
    pos = 0;
    line = 1;
}
````

It also defines an `InitScanner( const char* path )` function. This is the one 
which reads the given file in `path` and put it into the `buffer`.

```` C
Scanner InitScanner(const char* path) {
    Scanner scanner = {0 };

    FILE* file = fopen(path, "rb");
    if ( !file ) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    buffer_size = ftell(file);
    rewind(file);

    buffer = (char*) malloc(buffer_size + 1); /* +1 for null terminator */
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory for file\n");
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, buffer_size, file);
    buffer[buffer_size] = '\0'; /* Null-terminate the buffer */

    fclose(file);

    pos = 0;

    scanner.peek = &peek;
    scanner.match = &match;
    scanner.advance = &advance;
    scanner.next = &next;
    scanner.get_line_number = &get_line_number;
    scanner.destroy = &destroy;

    return scanner;
}
````

### Token API

The Token files defines one struct and one enum: 

```` C
typedef enum {
    /* One-character tokens : Tokens de un caracter */
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR, BIT_AND,
    BIT_OR, X_OR, NOT, DOTS, LEFT_BRACKET, RIGHT_BRACKET, QUESTION,

    /* Two-character tokens : Tokens de dos caracteres */
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESSER, LESSER_EQUAL,
    PLUS_EQUAL, MINUS_EQUAL,
    SLASH_EQUAL, STAR_EQUAL,
    AND, BIT_AND_EQUAL, OR, BIT_OR_EQUAL,
    ARROW, X_OR_EQUAL, DIFFERENT,
    PLUS_PLUS, MINUS_MINUS,

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

typedef struct Token {
    TokenType type;
    unsigned int line;
    unsigned int size; 
    char lexeme[100];
} Token;
````

The TokenType stores an int for every language lexeme. The Token struct stores the type, 
line, size and a char buffer for the lexeme. 

There are 4 function prototypes and 3 external variables. 

Variables: 

- `__TokenList` a list of tokens.
- `__Tokens` current amount of tokens in `__TokenList`.
- `__CurrentSize` how many token could get in `__TokenList`.

Functions:

- `printToken()` just prints a formatted token.
- `makeToke()` returns a Token struct instance given all the data needed for a token.
- `addToken()` adds a token to `__TokenList`.
- `freeTokenList()` frees the memory reserved for `__TokenList`. 

### Keywords API

This section was developed thinking in static search sets. The implementation of a static
search set is basically a perfect hash function. So, in order to create a fast and very 
specific map, we used `gperf` (https://www.gnu.org/software/gperf/) to generate a perfect 
hash function based on the C's keywords set. Take a look on the original article:
https://www.dre.vanderbilt.edu/~schmidt/PDF/gperf.pdf.

The generated hash function needs two parameters to work: a `char*` and its size. 
The only function to be used externally is `get_token()` which a returns a `TokenType`
associated with the keyword only if the `char*` passed and its size are equal to some of the 
`char*` in the set. 

``` C
typedef struct Bucket {
    char lBuffer[100];
    TokenType type;
} Bucket;

typedef struct Map {
    TokenType (*get_token) ( char*, unsigned int);
} Map;

extern Map InitMap ( void );
```

Bucket it's an auxiliary struct. Finally, if the given `char*` and its size
doesn't match with any of the given set, then the function returns `ERROR` as a `TokenType`. 

### Main Lexer

The main lexer works as follows:

```
while ( current_char != EOF ) do:

Read a character c: 
if c is # advance until '\n'
else if c is ' ' or '\t' or '\n' advance
else if c is '/' and the next character is '/' or '*' call function to handle comments
else if c is alpha call function to handle identifiers and keywords
else if c is number call function to handle numbers
else if c is " call function to handle strings
else if c is ' call function to handle characters
else c is either one-character or two-character token

end while

printTokens()
```

## Conclusions and further improving

This is a personal project for a college class. It could have some unexpected
bugs and errors. There's an issue when adding two-character tokens: I haven't 
found a way to store correctly the entire lexeme and its size without modifying
too much the code for adding a token or without using some extra variable. 

There's a probability that I will use this code for trying to code a compiler
so, if you notice any important bug (one that leads to a compilation error or
something like that) please, feel free to contact me or open a discussion in
this repository. 