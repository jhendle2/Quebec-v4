#ifndef QUEBEC_PARSER_H
#define QUEBEC_PARSER_H

#include <glib.h>

#include "ParseType.h"

typedef struct ParseNode ParseNode; 
struct ParseNode {
    ParseType type;
    GSList*   tokens;
    ParseNode *parent, *left, *right;
};

ParseNode* buildParseTree(GSList* tokens);
void fprintParseNode(FILE* fp, const ParseNode* node);
void fprintTreeParseNode(FILE* fp, const ParseNode* node, const int level);

#endif /* QUEBEC_PARSER_H */