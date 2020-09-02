#ifndef NAMESPACE
#define NAMESPACE

struct TokenList;
#include "../../../../ast/ast.h"
#include "../../../../ast/free_ast.h"

struct Namespace* makeNamespace(struct TokenList* tokens, char* name, bool debug);

void ns_parse_methods(struct Namespace* res, struct TokenList* copy, bool debug);
void ns_parse_structs(struct Namespace* res, struct TokenList* copy, bool debug);

#endif
