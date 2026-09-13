#ifndef QUEBEC_TOKEN_H
#define QUEBEC_TOKEN_H

#include <glib.h>
#include <stdio.h>

#include "Common/Constants.h"
#include "TokenType.h"
#include "Keywords.h"
#include "Operators.h"

typedef struct {
    // ------------------------------------------------
    int   row, col;
    char  text[MAX_TOKEN_LEN];
    const char* file_path;
    
    // ------------------------------------------------
    TokenType type;
    union {
        // ------------------------------------------------
        const char* const_string;
        char        const_char;
        int         const_int;
        int         const_oct;
        int         const_hex;
        float       const_float;

        // ------------------------------------------------
        Keyword     keyword;
        Operator    operator;

        // ------------------------------------------------
        const char* identifier;
    };
} Token;

void fprintToken(FILE* fp, const Token* token);
void fprintTokens(FILE* fp, GSList* tokens);

Token* newToken(
    const int row,
    const int col,
    const char* text,
    const char* file_path
);

void destroyToken(Token* token);

void destroyTokens(GSList* tokens);

#endif /* QUEBEC_TOKEN_H */