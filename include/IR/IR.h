#ifndef QUEBEC_IR_H
#define QUEBEC_IR_H

#include <glib.h>

#include "Parser/Parser.h"

#include "IRType.h"

typedef struct {
    IRType type;
    const ParseNode* node;
} IR;

GSList* generateIRList(const ParseNode* parse_tree);

void fprintIRList(FILE* fp, GSList* irs);

#endif /* QUEBEC_IR_H */