#include <stdio.h>
#include <stdlib.h>

#include "HexConst.h"

#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../../../ast/free_ast.h"
#include "../../../token/token.h"

struct HexConst* makeHexConst(struct TokenList* tokens, bool debug){
	
	struct HexConst* res = malloc(sizeof(struct HexConst));

	if(debug){
		printf("HexConst(...) from ");
		list_print(tokens);
	}

	struct Token* tk = list_head(tokens);
	
	if(tk == NULL){
		free(res);
		return NULL;
	}

	if(tk->kind == HEXCONST){
		
			//"0x10" -> 16, ...
			//use strtol to convert hex string -> int
			res->value = strtol(tk->value_ptr+2, NULL, 16);
			
			list_consume(tokens, 1);
			
	}else{
			free(res);
			return NULL;
	}

	return res;
}