#ifndef PARSER_H_
#define PARSER_H_

#include "lexer.h"
#include "vm.h"

#define STRLIB_NO_PREFIX
#include "strlib.h"

#define ALLOC_FROM_STACK -1

void Parse(Op **ops, Token *tokens);
#endif // PARSER_H_
