#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "IR/IR.h"

#include <ulog.h>
#include <argparse.h>

struct Args {
    const char* file_path;
    bool verbose;
};

static int parseArgs(const int argc, const char** argv, struct Args* args) {
    // ------------------------------------------------
    static const char *const usages[] = {
        "basic [options] [[--] args]",
        "basic [options]",
        NULL,
    };

    // ------------------------------------------------
    struct argparse_option options[] = {
        OPT_HELP(),
        
        OPT_GROUP("Basic Options"),
        OPT_BOOLEAN('v', "verbose", &args->verbose, "Enable verbose logging", NULL, 0, 0),
        OPT_STRING('f', "filepath", &args->file_path, "Input file path", NULL, 0, 0),
    };

    // ------------------------------------------------
    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nQuebec Compiler", "");
    const int remaining = argparse_parse(&argparse, argc, argv);

    // ------------------------------------------------
    if (args->file_path == NULL) {
        if (remaining<1) {
            ulog_error("No input file path provided.");
            argparse_usage(&argparse);
            return EXIT_FAILURE;
        } args->file_path = argv[0];
    } return EXIT_SUCCESS;
}

int main(int argc, const char** argv) {
    // ------------------------------------------------
    struct Args args = {0};
    if (EXIT_FAILURE == parseArgs(argc, argv, &args)) {
        return EXIT_FAILURE;
    }

    // ------------------------------------------------
    GSList* tokens = readFilePathAsTokens(args.file_path);

    ParseNode* parse_tree = buildParseTree(tokens);
    fprintTreeParseNode(stdout, parse_tree, 0);

    GSList* irs = generateIRList(parse_tree);
    fprintIRList(stdout, irs);

    destroyTokens(tokens);
    return EXIT_SUCCESS;
}