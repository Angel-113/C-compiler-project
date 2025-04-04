cmake_minimum_required (VERSION 3.29)
project (grammar C)

set (CMAKE_C_STANDARD 11)

include_directories (c-lex c-parser)



add_executable (grammar
        main.c
        c-lex/lexer.c
        c-lex/linklist.c
        c-lex/nfa.c
        c-lex/regex.c
        c-lex/Symbol.c
        c-lex/symtable.c
        c-parser/parser.c
        c-parser/parser.h
        c-parser/shift_reduce_parser.c)
