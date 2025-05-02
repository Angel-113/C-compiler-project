#include "keywords.h"

#define MAX_WORD_LENGTH 8
#define MIN_WORD_LENGTH 2
#define MAX_HASH_VALUE 54

static TokenType get_token ( char* lexeme, unsigned int size );
static void put ( char* lexeme, unsigned int size, TokenType type );

/* gperf generated functions */
static unsigned int hash( register const char *str, register size_t len );
static const char* in_word_set ( register const char *str, register size_t len );

static Bucket KBucket[MAX_HASH_VALUE + 1];

Map InitMap ( void ) {

    Map map = { 0 };
    map.get_token = &get_token;

    put("auto",     4, AUTO);
    put("break",    5, BREAK);
    put("case",     4, CASE);
    put("char",     4, CHAR);
    put("const",    5, CONST);
    put("continue", 8, CONTINUE);
    put("default",  7, DEFAULT);
    put("do",       2, DO);
    put("double",   6, DOUBLE);
    put("else",     4, ELSE);
    put("enum",     4, ENUM);
    put("extern",   6, EXTERN);
    put("float",    5, FLOAT);
    put("for",      3, FOR);
    put("goto",     4, GOTO);
    put("if",       2, IF);
    put("inline",   6, INLINE);
    put("int",      3, INT);
    put("long",     4, LONG);
    put("register", 8, REGISTER);
    put("restrict", 8, RESTRICT);
    put("return",   6, RETURN);
    put("short",    5, SHORT);
    put("signed",   6, SIGNED);
    put("sizeof",   6, SIZEOF);
    put("static",   6, STATIC);
    put("struct",   6, STRUCT);
    put("switch",   6, SWITCH);
    put("typedef",  7, TYPEDEF);
    put("union",    5, UNION);
    put("unsigned", 8, UNSIGNED);
    put("void",     4, VOID);
    put("volatile", 8, VOLATILE);
    put("while",    5, WHILE);

    return map;
}

void put ( char* lexeme, unsigned int size, TokenType type ) {
    unsigned int index = hash(lexeme, size);
    memcpy(KBucket[index].lBuffer, lexeme, (size + 1) * sizeof(char));
    KBucket[index].type = type;
}

TokenType get_token ( char* lexeme, unsigned int size ) {
    const char* s = in_word_set(lexeme, size);
    if (!s || *s == '\0') return ERROR;
    unsigned int index = hash(lexeme, size);
    return KBucket[index].type;
}

/* Static search set implemented with perfect hash function generated with gperf */

static unsigned int hash(register const char *str, register size_t len) {

    static const unsigned char asso_values[] = {
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 20, 0, 30,
            20, 20, 0, 10, 55, 5, 55, 55, 40, 55,
            0, 0, 5, 55, 0, 5, 0, 10, 0, 5,
            55, 55, 30, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55
    };

    register unsigned int hash_val = len;

    switch ( hash_val ) {
        default:
            hash_val += asso_values[( unsigned char ) str[2]];
            /*FALLTHROUGH*/
        case 2:
        case 1:
            hash_val += asso_values[( unsigned char ) str[0]];
            break;
    }

    return hash_val;
}

const char *in_word_set(register const char *str, register size_t len) {
    static const char *const wordlist[] = {
            "", "", "",
            "for",
            "",
            "float",
            "return",
            "if",
            "int",
            "void",
            "short",
            "struct",
            "typedef",
            "restrict",
            "goto",
            "while",
            "switch",
            "",
            "register",
            "",
            "union",
            "signed",
            "do",
            "unsigned",
            "auto",
            "break",
            "extern",
            "default",
            "",
            "else",
            "",
            "static",
            "", "",
            "enum",
            "const",
            "double",
            "",
            "continue",
            "case",
            "",
            "sizeof",
            "", "",
            "long",
            "", "", "",
            "volatile",
            "", "",
            "inline",
            "", "",
            "char"
    };

    if ( len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH ) {
        register unsigned int key = hash(str, len);
        if ( key <= MAX_HASH_VALUE ) {
            register const char *s = wordlist[key];
            if ( *str == *s && !strcmp(str + 1, s + 1))
                return s;
        }
    }
    return 0;
}