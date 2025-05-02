#include "scanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* buffer = NULL;
static size_t buffer_size = 0;
static unsigned int pos = 0;
static unsigned int line = 1;

static char peek(void) {
    return buffer[pos];
}

static bool match(char expected) {
    if ( buffer[pos] == '\0' || buffer[pos] != expected ) return false;
    pos++;
    return true;
}

static char advance(void) {
    if (buffer[pos] == '\0') return '\0'; /* End of file */
    char c = buffer[pos++];
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

static char next ( void ) {
    if (buffer[pos] == '\0') return '\0';
    return buffer[pos + 1];
}

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

