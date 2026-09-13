#include "Lexer/Token.h"
#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <ulog.h>

void fprintToken(FILE* fp, const Token* token) {
    fprintf(fp,
        "%s:%d:%d: %s <%s"
        , token->file_path
        , token->row
        , token->col
        , token->text
        , strTokenType[token->type]
    );

    switch (token->type) {
        case TokenTypeIdentifier: { fprintf(fp, ">\n"); } break;
        case TokenTypeOperator  : { fprintf(fp, ": %s>\n", strOperator[token->operator]); } break;
        case TokenTypeKeyword   : { fprintf(fp, ": %s>\n", strKeyword[token->keyword]); } break;
        case TokenTypeConstChar : { fprintf(fp, ": %c>\n", token->const_char); } break;
        case TokenTypeConstInt  : { fprintf(fp, ": %d>\n", token->const_int); } break;
        case TokenTypeConstHex  : { fprintf(fp, ": %x>\n", token->const_hex); } break;
        case TokenTypeConstOct  : { fprintf(fp, ": %o>\n", token->const_oct); } break;
        case TokenTypeConstFloat: { fprintf(fp, ": %f>\n", token->const_float); } break;
        default: { fprintf(fp, ">\n"); } break;
    }
}
void fprintTokens(FILE* fp, GSList* tokens) {
    for (
        GSList* iter = tokens;
        iter;
        iter = iter->next
    ) {
        fprintToken(
            fp, iter->data
        );
    }
}

Token* newToken(
    const int row,
    const int col,
    const char* text,
    const char* file_path
) {
    Token* token = g_new(Token, 1);
    token->row = row;
    token->col = col;
    token->file_path = file_path;
    strncpy(
        token->text,
        text,
        MAX_TOKEN_LEN-1
    );

    // ----------------------------------------------------------------
    token->type = deduceTokenType(token->text);
    switch (token->type) {
        case TokenTypeOperator   : { token->operator     = getOperator(token->text); } break;
        case TokenTypeKeyword    : { token->keyword      = getKeyword(token->text); } break;
        case TokenTypeConstOct   : { token->const_oct    = strtoll(token->text, NULL, 8); } break;
        case TokenTypeConstInt   : { token->const_int    = strtoll(token->text, NULL, 10); } break;
        case TokenTypeConstHex   : { token->const_hex    = strtoll(token->text, NULL, 16); } break;
        case TokenTypeConstFloat : { token->const_float  = strtof(token->text, NULL); } break;
        case TokenTypeConstString: { token->const_string = token->text; } break;
        case TokenTypeConstChar  : { token->const_char   = getChar(token->text); } break;

        default: break;
    }

    return token;
}

void destroyToken(Token* token) {
    g_free(token);
}

void destroyTokens(GSList* tokens) {
    int DBG_destroyed_tokens = 0;
    for (
        GSList* iter = tokens;
        iter;
        iter = iter->next
    ) {
        destroyToken(iter->data);
        DBG_destroyed_tokens++;
    } g_slist_free(tokens);
    ulog_debug("Destroyed (%d) tokens", DBG_destroyed_tokens);
    ulog_debug("Destroyed (1) GSList*: tokens");
}