cmake_minimum_required (VERSION 3.29)
project (grammar C)

set (CMAKE_C_STANDARD 11)

include_directories (include src/c-lex src/c-parser src/c-semantic)

add_executable ( grammar
        src/main.c
        src/c-lex/lexer.c
        src/c-lex/lexer.h
        src/c-lex/token.c
        src/c-lex/token.h
        include/base.h
        src/c-lex/scanner.c
        src/c-lex/scanner.h
        src/c-lex/keywords.c
        src/c-lex/keywords.h
        src/c-lex/test/test.c
)
