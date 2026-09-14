#ifndef QUEBEC_PARSERULES_H
#define QUEBEC_PARSERULES_H

#include "ParseType.h"

static inline ParseType reduceParseType1(const ParseType type) {
    #define LR0(RESULT,TYPE) if (type==ParseType_##TYPE) return ParseType_##RESULT;
        #include "LR0.inc"
    #undef LR0
    return type;
}

static inline ParseType reduceParseType2(const ParseType left, const ParseType right) {
    #define LR1(RESULT,LEFT,RIGHT) if (left==ParseType_##LEFT && right==ParseType_##RIGHT) return ParseType_##RESULT;
        #include "LR1.inc"
    #undef LR1
    return ParseTypeUndefined;
}

static inline bool lookAheadIgnore3(const ParseType left, const ParseType right, const ParseType next) {
    #define LALR(LEFT, RIGHT, NEXT) if (left==ParseType_##LEFT && right==ParseType_##RIGHT && next==ParseType_##NEXT) return true;
        #include "LALR.inc"
    #undef LALR
    return false;
}

#endif /* QUEBEC_PARSERULES_H */