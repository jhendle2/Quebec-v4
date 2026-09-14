#ifndef QUEBEC_IRTYPE_H
#define QUEBEC_IRTYPE_H

#define IRTypes \
    IRType(Noop) \
    IRType(Namespace) \
    IRType(Function) \
    IRType(Declare) \
    IRType(Return) \
    IRType(Call) \
    IRType(Vlookup) \
    IRType(ArgsBegin) \
    IRType(ArgsEnd) \
    IRType(ArgsNext) \
    IRType(LoadConstant) \
    IRType(Leave) /* Leave a function body */ \
// IRTypes

typedef enum {
    IRTypeUndefined=0,
        #define IRType(E) IRType##E,
        IRTypes
        #undef IRType
    NumIRTypes,
} IRType;

static const char* strIRType[NumIRTypes] = {
    "Undefined",
    #define IRType(E) #E,
        IRTypes
    #undef IRType
};

#endif /* QUEBEC_IRTYPE_H */