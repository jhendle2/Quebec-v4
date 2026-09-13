#ifndef QUEBEC_LEXER_H
#define QUEBEC_LEXER_H

#include <glib.h>
#include <stdio.h>
#include "Token.h"

GSList* readFilePathAsTokens(const char* file_path);

#endif /* QUEBEC_LEXER_H */