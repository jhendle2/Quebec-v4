#include "Lexer/Lexer.h"

#include <ctype.h>
#include <glib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <ulog.h>

static FILE* openFilePathOrFail(const char* file_path, const char* mode) {
    FILE* fp = fopen(file_path, mode);
    if (fp == NULL) {
        ulog_error("failed to open file path: %s as [%s]"
            , file_path
            , mode
        ); exit(EXIT_FAILURE); // FIXME: Should really propagate the error upwards...
    } return fp;
}

static inline size_t getFileSize(FILE* fp) {
    // Ensures we rewind the file both before and after sizing
    rewind(fp);
    fseek(fp, 0, SEEK_END);
    const size_t file_size = ftell(fp);
    rewind(fp);
    return file_size;
}

char* readFileAsString(const char* file_path) {
    FILE* fp = openFilePathOrFail(file_path, "r");
    const size_t file_size = getFileSize(fp);

    char* file_text = g_malloc(file_size+1);
    fread(file_text, 1, file_size, fp);
    file_text[file_size] = 0;
    fclose(fp);

    return file_text;
}

static void _pushBackToken(
    GSList** tokens,
    char text[MAX_TOKEN_LEN],
    size_t* text_len,

    const int row,
    const int col,
    const char* file_path
) {
    if ((*text_len)==0) return;

    // text[(*text_len)] = 0;
    (*tokens) = g_slist_append(
        (*tokens),
        newToken(
            row,
            col,
            text,
            file_path
        )
    );
    text[(*text_len) = 0] = 0;
}

static void _appendChar(
    GSList** tokens,
    char text[MAX_TOKEN_LEN],
    size_t* text_len,

    const int row,
    const int col,
    const char* file_path,

    const char c,

    bool* in_char,
    bool* in_string,

    bool* in_line_comment,
    bool* in_block_comment
) {
    // ------------------------------------------------
    #define _clearText() text[(*text_len)=0] = 0
    #define _pushBackToken() _pushBackToken(\
        tokens,\
        text,\
        text_len,\
        row,\
        col,\
        file_path\
    )
    
    // We need to null terminate here for checking possible len>=2 operators and identifiers
    #define __appendChar(c) do { text[(*text_len)++] = c; text[(*text_len)] = 0; } while (0)
    #define __popBackText(c) text[--(*text_len)] = 0

    // ------------------------------------------------
    // Line Commments
    if ((*in_line_comment) && c=='\n') {
        (*in_line_comment) = false;
        return;
    }
    if (getOperator(text) == OperatorCommentLine) {
        (*in_line_comment) = true;
        _clearText();
        return;
    }
    if (getOperator(text) == OperatorHash) { // TODO: Remove
        (*in_line_comment) = true;
        _clearText();
        return;
    }
    if ((*in_line_comment)) return;

    // ------------------------------------------------
    // Block Commments
    if (getOperator(text) == OperatorCommentOpen) {
        (*in_block_comment) = true;
        _clearText();
        return;
    }
    if ((*in_block_comment)) return;


    // ------------------------------------------------
    // String combining
    if (!(*in_char) && c=='\"') {
        if (*in_string) __appendChar(c);
        _pushBackToken();
        if (!(*in_string)) __appendChar(c);
        (*in_string) = !(*in_string);
        return;
    }
    
    // ------------------------------------------------
    // Character combining
    if (!(*in_string) && c=='\'') {
        if (*in_char) __appendChar(c);
        _pushBackToken();
        if (!(*in_char)) __appendChar(c);
        (*in_char) = !(*in_char);
        return;
    }
    
    // ------------------------------------------------
    // Space delimiting
    if (!(*in_char) && !(*in_string) && isWhitespace(c)) {
        _pushBackToken();
        return;
    }

    // ------------------------------------------------
    // Float preservation
    if (isConstInt(text) && c=='.') {
        __appendChar(c);
        return;
    }

    // ------------------------------------------------
    typedef bool (*Pred)(const char*);
    static Pred preds[] = {
        isOperator,
        isKeyword,
        isPossibleIdentifier,
        isConstInt,
        isConstOct,
        isConstHex,
        isConstFloat,
    };

    for (size_t i = 0; i<sizeof(preds)/sizeof(Pred); i++) {
        if (preds[i](text)) {
            __appendChar(c);
            if (preds[i](text)) return;
            __popBackText();
            _pushBackToken();
            __appendChar(c);
            return;
        }
    }
    
    __appendChar(c);

    #undef _pushBackToken
}

GSList* readStringAsTokens(const char* file_path, const char* string) {
    ulog_debug("file_text:\n```%s```\n", string);

    GSList* tokens = NULL;
    char text[MAX_TOKEN_LEN];
    size_t text_len = 0;

    int row = 0, col = 0;
    bool in_string = false, in_char = false;
    bool in_line_comment = false, in_block_comment = false;

    #define _appendChar(C) _appendChar(\
        &tokens,\
        text,\
        &text_len,\
        row,\
        col,\
        file_path,\
        C,\
        &in_char,\
        &in_string,\
        &in_line_comment,\
        &in_block_comment\
    )

    for (const char* t = string; t && *t; t++) {
        if (*t=='*' && *(t+1)=='/') {
            in_block_comment = false;
            t++;
            continue;
        }
        _appendChar(*t);
    }
    
    _pushBackToken(
        &tokens,
        text,
        &text_len,
        row,
        col,
        file_path
    );

    return tokens;
}

GSList* readFilePathAsTokens(const char* file_path) {
    char* file_text = readFileAsString(file_path);
    GSList* tokens = readStringAsTokens(file_path, file_text);
    // fprintTokens(stdout, tokens);
    g_free(file_text);
    return tokens;
}