#include "Parser/Parser.h"
#include <glib.h>
#include <stdio.h>
#include <ulog.h>

#include "Parser/ParseRules.h"

ParseNode* tokenToParseNode(const Token* token) {
    ParseNode* node = g_new(ParseNode, 1);
    node->type = tokenToParseType(token);
    node->left = node->right = node->parent = NULL;
    return node;
}

ParseNode* combineParseNodes2(const ParseType combined_type, __attribute__((unused)) ParseNode* left, __attribute__((unused)) ParseNode* right) {
    ParseNode* combined_node = g_new(ParseNode, 1);
    combined_node->type = reduceParseType1(combined_type);
    combined_node->left = combined_node->right = combined_node->parent = NULL;
    // FIXME:
    // Steal left's tokens
    // combined_node->tokens = left->tokens;
    // left->tokens = NULL;

    // // Steal right's tokens
    // combined_node->tokens = g_slist_concat(combined_node->tokens, right->tokens);
    // right->tokens = NULL;

    return combined_node;
}

void fprintTreeParseNode(FILE* fp, const ParseNode* node, const int level) {
    if (node == NULL) return;
    for (int i = 0; i<level; i++) fprintf(fp, " *");
    fprintf(fp, " %s\n", strParseType[node->type]);
    fprintTreeParseNode(fp, node->left, level+1);
    fprintTreeParseNode(fp, node->right, level+1);
}

// static gint g_compareParseNodes(gconstpointer a, gconstpointer b, __attribute__((unused)) gpointer user_data) {
//     return a==b;
// }

void destroyParseNode(ParseNode* node) {
    g_free(node);
}

void destroyParseNodeStack(GQueue* stack) {
    for (
        GList* iter = stack->head;
        iter;
        iter = iter->next 
    ) {
        destroyParseNode(iter->data);
    }
    g_queue_free(stack);
}

void DEBUG_dumpParseStack(GQueue* stack) {
    fprintf(stdout, "Stack (%d): ", g_queue_get_length(stack));
    for (
        GList* iter = stack->head;
        iter;
        iter = iter->next 
    ) { 
        fprintParseNode(stdout, iter->data);
    } fprintf(stdout, "\n");
}

static ParseNode* reduce2FromStack(GQueue* stack, const ParseType lookahead_type) {
    ParseNode* reduced_node = NULL;
    while (g_queue_get_length(stack)>=2) {
        ParseNode* right = g_queue_pop_tail(stack);
        ParseNode* left  = g_queue_pop_tail(stack);

        const char* DEBUG_parse_type_msg = "LR1";
        char DEBUG_parse_expr_msg[MAX_DEBUG_MSG_LEN] = "";
        if (lookahead_type!=ParseTypeUndefined) {
            DEBUG_parse_type_msg = "LALR";
            snprintf(DEBUG_parse_expr_msg, MAX_DEBUG_MSG_LEN-1,
                "`%s` + `%s` (+ `%s`)"
                , strParseType[left->type]
                , strParseType[right->type]
                , strParseType[lookahead_type]
            );
        } else {
            snprintf(DEBUG_parse_expr_msg, MAX_DEBUG_MSG_LEN-1,
                "`%s` + `%s`"
                , strParseType[left->type]
                , strParseType[right->type]
            );
        }

        const ParseType reduced_type = reduceParseType2(left->type, right->type);
        bool lookahead_ignore = false;
        if (lookahead_type!=ParseTypeUndefined) {
            lookahead_ignore = lookAheadIgnore3(left->type, right->type, lookahead_type);
        }
        if (reduced_type == ParseTypeUndefined || lookahead_ignore != ParseTypeUndefined) {
            g_queue_push_tail(stack, left);
            g_queue_push_tail(stack, right);
            break;
        }

        ulog_debug("%-4s : `%s` ::= %s"
            , DEBUG_parse_type_msg
            , strParseType[reduced_type]
            , DEBUG_parse_expr_msg
        );
        reduced_node = combineParseNodes2(
            reduced_type,
            left,
            right
        );

        reduced_node->left  = left;
        reduced_node->right = right;
        
        g_queue_push_tail(stack, reduced_node);
    } return reduced_node;
}

ParseNode* reduce1FromStack(GQueue* stack) {
    ParseNode* parse_node = stack->head->data;
    ParseType type = parse_node->type;
    ParseType reduced_type = ParseTypeUndefined;
    ParseNode* reduced_node = NULL;

    while (reduced_type != type) {
        reduced_node = combineParseNodes2(reduced_type, NULL, NULL);
        reduced_node->left = parse_node;
        parse_node = reduced_node;
        type = reduced_type;
    } return reduced_node;
}

ParseNode* buildParseTree(GSList* tokens) {
    ParseNode* tree = NULL;
    GQueue* stack = g_queue_new();

    GSList* iter = tokens;
    do {
        const Token* token = iter->data;
        ParseNode* node = tokenToParseNode(token);
        g_queue_push_tail(stack, (gpointer)node);

        DEBUG_dumpParseStack(stack);

        ParseType lookahead_type = ParseTypeUndefined;
        if (iter->next != NULL && iter->next->data != NULL) {
            Token* next = iter->next->data;
            lookahead_type = tokenToParseType(next);
        }
        
        ParseNode* reduced = reduce2FromStack(stack, lookahead_type);
        if (reduced != NULL) tree = reduced;
        
        iter = iter->next;
    } while (iter != NULL);
    
    ulog_debug("Done consuming tokens...");
    DEBUG_dumpParseStack(stack);
    
    ulog_debug("Reducing stack...");
    ParseNode* reduced2 = reduce2FromStack(stack, ParseTypeUndefined);
    if (reduced2 != NULL) tree = reduced2;
    
    ParseNode* reduced1 = reduce1FromStack(stack); // FIXME: Is this still needed?
    if (reduced1 != NULL) tree = reduced1;
    
    ulog_debug("Done reducing stack...");
    DEBUG_dumpParseStack(stack);
    // destroyParseNodeStack(stack);

    tree->type = ParseType_PT_Namespace;
    return tree;
}

void fprintParseNode(FILE* fp, const ParseNode* node) {
    fprintf(fp, "`%s` ", strParseType[node->type]);
}