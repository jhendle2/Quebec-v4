#include "IR/IR.h"
#include <glib.h>

#include <ulog.h>

IR* newIR(const IRType type, const ParseNode* parse_node) {
    IR* ir = g_new(IR, 1);
    ir->node = parse_node;
    ir->type = type;
    return ir;
}

GSList* generateIRArg(const ParseNode* arg) {
    GSList* irs = NULL;

    ParseNode* argument = arg->left;
    ulog_debug("argument = %s", strParseType[argument->type]);
    if (argument->type == ParseType_PT_Constant) {
        irs = g_slist_append(irs, newIR(IRTypeArgsNext, argument));
        irs = g_slist_append(irs, newIR(IRTypeVlookup, argument));
    }

    return irs;
}

GSList* generateIRArgs(const ParseNode* args) {
    GSList* irs = NULL;

    irs = g_slist_append(irs, newIR(IRTypeArgsBegin, args));
    irs = g_slist_concat(irs, generateIRArg(args->right));
    irs = g_slist_append(irs, newIR(IRTypeArgsEnd, args));

    return irs;
}

GSList* generateIRFnCall(const ParseNode* fn_call) {
    GSList* irs = NULL;
    const ParseNode* fn_identifier = fn_call->left;
    const ParseNode* fn_args = fn_call->right;

    irs = g_slist_concat(irs, generateIRArgs(fn_args));
    irs = g_slist_append(irs, newIR(IRTypeCall, fn_identifier));

    return irs;
}

GSList* generateIR(const ParseNode* parse_node) {
    if (parse_node == NULL) return NULL;
    GSList* irs = NULL;

    switch (parse_node->type) {
        case ParseType_PT_Namespace: {
            irs = g_slist_append(irs, newIR(IRTypeNamespace, parse_node));
        } break;

        case ParseType_PT_FnDefn: {
            irs = g_slist_append(irs, newIR(IRTypeFunction, parse_node));
        } break;

        case ParseType_KW_Return: {
            irs = g_slist_append(irs, newIR(IRTypeReturn, parse_node));
        } break;

        case ParseType_PT_FnCall: {
            irs = g_slist_concat(irs, generateIRFnCall(parse_node));
        } break;

        case ParseType_PT_Constant: {
            irs = g_slist_append(irs, newIR(IRTypeLoadConstant, parse_node));
        } break;

        default: {
            irs = g_slist_append(irs, newIR(IRTypeNoop, parse_node));
        } break;
    }

    return irs;
}

GSList* _generateIRList(const ParseNode* parse_node) {
    if (parse_node == NULL) return NULL;
    GSList* irs = NULL;
    irs = g_slist_concat(irs, generateIR(parse_node));
    irs = g_slist_concat(irs, _generateIRList(parse_node->left));
    irs = g_slist_concat(irs, _generateIRList(parse_node->right));

    if (parse_node->type == ParseType_PT_BodyEnd) {
        irs = g_slist_append(irs, newIR(IRTypeLeave, parse_node));
    }

    return irs;
}

GSList* generateIRList(const ParseNode* parse_tree) {
    return _generateIRList(parse_tree);
}

void fprintIR(FILE* fp, const IR* ir, const int index) {
    fprintf(fp, "%4d: %-16s: ", index, strIRType[ir->type]);
    if (ir->node == NULL) {
        fprintf(fp, "(null-node)\n");
    } else {
        fprintf(fp, "%-16s: ", strParseType[ir->node->type]);
        flistTokens(fp, ir->node->tokens);
        fprintf(fp, "\n");
    }
}

void fprintIRList(FILE* fp, GSList* irs) {
    int index = 0;
    for (
        GSList* iter = irs;
        iter;
        iter = iter->next
    ) {
        const IR* ir = iter->data;
        fprintIR(fp, ir, index++);
    }
}