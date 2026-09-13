#include "Lexer/TokenType.h"

#include <stddef.h>
#include <string.h>

#include <ulog.h>

#include "Lexer/Keywords.h"
#include "Lexer/Operators.h"

#include "Common/Constants.h"
#include "Common/Utils.h"

static inline bool isAlpha     (const char c) { return (c>='A' && c<='Z') || (c>='a' && c<='z'); }

static inline bool isDigit8    (const char c) { return (c>='0' && c<='7'); }
static inline bool isDigit10   (const char c) { return (c>='0' && c<='9'); }
static inline bool isDigit16   (const char c) { return (c>='0' && c<='9') || (c>='a' && c<='f') || (c>='A' && c<='F'); }

static inline bool isAlUnder   (const char c) { return c=='_' || isAlpha(c); }
static inline bool isAlNum     (const char c) { return isAlpha(c) || isDigit10(c); }
static inline bool isAlNumUnder(const char c) { return isAlUnder(c) || isDigit10(c); }

static inline bool v_isAlNumUnder(const void* v) { return isAlNumUnder(*(const char*)v); }
static inline bool v_isDigit8    (const void* v) { return isDigit8    (*(const char*)v); }
static inline bool v_isDigit10   (const void* v) { return isDigit10   (*(const char*)v); }
static inline bool v_isDigit16   (const void* v) { return isDigit16   (*(const char*)v); }

bool isPossibleIdentifier(const char* string) {
    const int len = strnlen(string, MAX_TOKEN_LEN-1);
    return (
        string!=NULL
        && len>0
        && isAlUnder(string[0])
        && all(
            (void*)(string+1),
            (void*)(string+len),
            v_isAlNumUnder
        )
    );
}

bool isConstInt(const char* string) {
    const int len = strnlen(string, MAX_TOKEN_LEN-1);
    return (
        string!=NULL
        && len>0
        && all(
            (void*)(string+0),
            (void*)(string+len),
            v_isDigit10
        )
    );
}

bool isConstOct(const char* string) {
    #define PREFIX_OCT "0"
    const int len = strnlen(string, MAX_TOKEN_LEN-1);
    return (
        string!=NULL
        && len>0
        && strncmp(string, PREFIX_OCT, 1)==0
        && all(
            (void*)(string+1),
            (void*)(string+len),
            v_isDigit8
        )
    );
}

bool isConstHex(const char* string) {
    #define PREFIX_HEX "0x"
    const int len = strnlen(string, MAX_TOKEN_LEN-1);
    return (
        string!=NULL
        && len>2
        && strncmp(string, PREFIX_HEX, 2)==0
        && all(
            (void*)(string+2),
            (void*)(string+len),
            v_isDigit16
        )
    );
}

bool isConstFloat(const char* string) {
    bool decimal = false;
    for (
        const char *t = string;
        t && *t;
        t++
    ) {
        if (!decimal && *t == '.') {
            decimal = true;
            continue;
        }
        if (!isDigit10(*t)) return false;
    } return decimal;
}

bool isConstString(const char* string) {
    const int len = strnlen(string, MAX_TOKEN_LEN-1);
    return (
        len>=2
        && string[0]=='\"'
        && string[len-1]=='\"'
    );
}

bool isConstChar(const char* string) {
    const int len = strnlen(string, MAX_TOKEN_LEN-1);
    return (
        len>=2
        && string[0]=='\''
        && string[len-1]=='\''
    );
}

char getChar(const char* string) {
    // Escape characters
    if (string[1]=='\\') {
        return string[2];
    } return string[1];
}

// ================================================
TokenType deduceTokenType(const char* string) {
    if (isKeyword(string)) return TokenTypeKeyword;
    if (isOperator(string)) return TokenTypeOperator;

    if (isConstInt   (string)) return TokenTypeConstInt;
    if (isConstOct   (string)) return TokenTypeConstOct;
    if (isConstHex   (string)) return TokenTypeConstHex;
    if (isConstFloat (string)) return TokenTypeConstFloat;

    if (isConstString(string)) return TokenTypeConstString;
    if (isConstChar  (string)) return TokenTypeConstChar;

    if (isPossibleIdentifier(string)) return TokenTypeIdentifier;

    ulog_error("Unsupported token type: %s", string);
    return TokenTypeUndefined;
}
