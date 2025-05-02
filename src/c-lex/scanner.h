#ifndef GRAMMAR_SCANNER_H
#define GRAMMAR_SCANNER_H

#include "base.h"

typedef struct Scanner {
    char (*peek)(void);
    bool (*match)(char);
    char (*advance)(void);
    char (*next)(void);
    unsigned int (*get_line_number)(void);
    void (*destroy)(void);
} Scanner;

extern Scanner InitScanner(const char* path);

#endif //GRAMMAR_SCANNER_H
