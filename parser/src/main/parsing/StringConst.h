#ifndef STRINGCONSTPARSER
#define STRINGCONSTPARSER

struct TokenList;

#include "../../../../ast/ast.h"

struct StringConst* makeStringConst(struct TokenList* tokens, bool debug);

#endif
