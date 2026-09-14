#ifndef QUEBEC_TOKENTYPE_H
#define QUEBEC_TOKENTYPE_H

#include <stdbool.h>

// ------------------------------------------------
#define TokenTypes         \
    TokenType(Operator)    \
    TokenType(Keyword)     \
    TokenType(Identifier)  \
    TokenType(ConstInt)    \
    TokenType(ConstHex)    \
    TokenType(ConstChar)   \
    TokenType(ConstString) \
    TokenType(ConstFloat)  \
    TokenType(ConstOct)  \
// TokenTypes

typedef enum {
    TokenTypeUndefined=0,
        #define TokenType(E) TokenType##E,
        TokenTypes
        #undef TokenType
    NumTokenTypes,
} TokenType;

__attribute__((unused))
static const char* strTokenType[NumTokenTypes] = {
    "Undefined",
    #define TokenType(E) #E,
    TokenTypes
    #undef TokenType
};

// ------------------------------------------------
// TODO: Only expose these thru extern w/ unit tests
bool isPossibleIdentifier(const char* string);
bool isConstInt(const char* string);
bool isConstOct(const char* string);
bool isConstHex(const char* string);
bool isConstFloat(const char* string);

#define isWhitespace(C) ((C)==' ' || (C)=='\t' || (C)=='\n' || (C)=='\v')

char getChar(const char* string);

// ------------------------------------------------
TokenType deduceTokenType(const char* string);

#endif /* QUEBEC_TOKENTYPE_H */