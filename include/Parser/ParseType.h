#ifndef QUEBEC_PARSETYPES_H
#define QUEBEC_PARSETYPES_H

#include <stdbool.h>

#include "Lexer/TokenType.h"
#include "Lexer/Token.h"

#define ParseTypes \
    ParseType(Namespace) \
    ParseType(Type) \
    ParseType(Decl) \
    ParseType(Stmt) \
    ParseType(Stmts) \
    ParseType(Constant) \
    ParseType(Expr) \
    ParseType(BodyBegin) \
    ParseType(BodyEnd) \
    ParseType(Body) \
    ParseType(Bodies) \
    ParseType(Args) \
    ParseType(ArgsBody) \
    ParseType(ArgsEnd) \
    ParseType(FnBody) \
    ParseType(FnDecl) \
    ParseType(FnDefn) \
    ParseType(TypeFnBody) \
    ParseType(FnCall) \
    ParseType(Binop) \
    ParseType(Monop) \
    ParseType(IdStmt) \
    ParseType(VarDecl) \
    ParseType(VarDefn) \
    ParseType(RValue) \
// ParseTypes

typedef enum {
    ParseTypeUndefined=0,
        #define ParseType(E) ParseType_PT_##E,
            ParseTypes
        #undef ParseType

        #define TokenType(E) ParseType_TT_##E,
            TokenTypes
        #undef TokenType

        #define Keyword(E,S) ParseType_KW_##E,
            #include "Lexer/Keywords.inc"
        #undef Keyword

        #define Operator(E,S) ParseType_OP_##E,
            #include "Lexer/Operators.inc"
        #undef Operator
    NumParseTypes,
} ParseType;

__attribute__((unused))
static const char* strParseType[NumParseTypes] = {
    "Undefined",

    #define ParseType(E) "PT_"#E,
        ParseTypes
    #undef ParseType

    #define TokenType(E) "TT_"#E,
        TokenTypes
    #undef TokenType

    #define Keyword(E,S) "KW_"#E,
        #include "Lexer/Keywords.inc"
    #undef Keyword

    #define Operator(E,S) "OP_"#E,
        #include "Lexer/Operators.inc"
    #undef Operator
};

ParseType fullyReduceParseType(ParseType type);
ParseType tokenToParseType(const Token* token);

#endif /* QUEBEC_PARSETYPES_H */