#include <string.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "UnOpTerm.h"

#include "Op.h"
#include "Term.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/token/token.h"

struct UnOpTerm* makeUnOpTerm(struct TokenList* tokens) {
	
	if(list_size(tokens) == 0){ return NULL; }
	
	struct UnOpTerm* res = make(UnOpTerm);
	
	struct TokenList* copy = list_copy(tokens);
	
	parse_astnode(copy, &(res->super));
	
	res->op = makeOp(copy);
	//res->op may be NULL, it is not a problem,
	//as the unary operator is optional
	
	if(res->op != NULL){
		
		//check if it is unary operator.
		//sometimes it inserts a space 
		//so check for that case also
		if(
			strcmp(res->op->op, "!") != 0 && 
			strcmp(res->op->op, "! ") != 0 && 
			strcmp(res->op->op, "~") != 0 &&
			strcmp(res->op->op, "~ ") != 0 &&
			strcmp(res->op->op, "-") != 0 &&
			strcmp(res->op->op, "- ") != 0
		){
			//the operator was not unary,
			//so we do not have an unary op term
			freeTokenListShallow(copy);
			free(res);
			return NULL;
		}
	}
	
	res->term = makeTerm(copy);
	
	if(res->term == NULL){
		free_op(res->op);
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}
	
	list_set(tokens, copy);
	
	freeTokenListShallow(copy);
	
	return res;
}


