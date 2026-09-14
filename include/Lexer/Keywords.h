#ifndef QUEBEC_KEYWORDS_H
#define QUEBEC_KEYWORDS_H

#include <stdbool.h>
#include <string.h>

#include "Common/Constants.h"

typedef enum {
    KeywordUndefined=0,
        #define Keyword(E,S) Keyword##E,
            #include "Keywords.inc"
        #undef Keyword
    NumKeywords,
} Keyword;

__attribute__((unused))
static const char* strKeyword[NumKeywords] = {
    "Undefined",
    #define Keyword(E,S) #E,
        #include "Keywords.inc"
    #undef Keyword
};
    
static inline Keyword getKeyword(const char* s) {      
    #define Keyword(E,S) if (strncmp(s, S, MAX_TOKEN_LEN-1)==0) return Keyword##E;
        #include "Keywords.inc"
    #undef Keyword
    return KeywordUndefined;
}

static inline bool isKeyword(const char* s) { return getKeyword(s) != KeywordUndefined; }

#endif /* QUEBEC_KEYWORDS_H */