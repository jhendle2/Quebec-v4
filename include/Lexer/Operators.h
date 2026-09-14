#ifndef QUEBEC_OPERATORS_H
#define QUEBEC_OPERATORS_H

#include <stdbool.h>
#include <string.h>

#include "Common/Constants.h"

typedef enum {
    OperatorUndefined=0,
        #define Operator(E,S) Operator##E,
            #include "Operators.inc"
        #undef Operator
    NumOperators,
} Operator;

__attribute__((unused))
static const char* strOperator[NumOperators] = {
    "Undefined",
    #define Operator(E,S) #E,
        #include "Operators.inc"
    #undef Operator
};
    
static inline Operator getOperator(const char* s) {      
    #define Operator(E,S) if (strncmp(s, S, MAX_TOKEN_LEN-1)==0) return Operator##E;
        #include "Operators.inc"
    #undef Operator
    return OperatorUndefined;
}

static inline bool isOperator(const char* s) { return getOperator(s) != OperatorUndefined; }

#endif /* QUEBEC_OPERATORS_H */