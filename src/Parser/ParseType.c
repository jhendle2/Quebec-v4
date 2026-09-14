#include "Parser/ParseType.h"

#include "Parser/ParseRules.h"

#include <ulog.h>

ParseType fullyReduceParseType(ParseType type) {
    ParseType new_type = ParseTypeUndefined;
    while (new_type != type) {
        new_type = reduceParseType1(type);
        if (new_type == type) break;
        
        ulog_debug("%-4s : `%s` ::= `%s`", "LR0", strParseType[new_type], strParseType[type]);
        type     = new_type;
    } return new_type;
}

static ParseType _tokenToParseType(const Token* token) {
    #define Keyword(E,S) if (token->type==TokenTypeKeyword && token->keyword==Keyword##E) return ParseType_KW_##E;
        #include "Lexer/Keywords.inc"
    #undef Keyword

    #define Operator(E,S) if (token->type==TokenTypeOperator && token->operator==Operator##E) return ParseType_OP_##E;
        #include "Lexer/Operators.inc"
    #undef Operator

    switch (token->type) {
        case TokenTypeIdentifier : return ParseType_TT_Identifier;
        case TokenTypeConstInt   : return ParseType_TT_ConstInt;
        case TokenTypeConstHex   : return ParseType_TT_ConstHex;
        case TokenTypeConstChar  : return ParseType_TT_ConstChar;
        case TokenTypeConstString: return ParseType_TT_ConstString;
        case TokenTypeConstFloat : return ParseType_TT_ConstFloat;
        case TokenTypeConstOct   : return ParseType_TT_ConstOct;
        default: break;
    }

    return ParseTypeUndefined;
}


ParseType tokenToParseType(const Token *token) {
    return fullyReduceParseType(
        _tokenToParseType(token)
    );
}