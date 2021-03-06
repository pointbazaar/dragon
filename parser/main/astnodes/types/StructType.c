#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "StructType.h"

#include "ast/ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

static void parse_type_params_rest(struct StructType* res, struct TokenList* tokens);

struct StructType* makeStructType(struct TokenList* tokens) {
	
	struct StructType* res = make(StructType);
	struct TokenList* copy = list_copy(tokens);
	
	parse_astnode(copy, &(res->super));
	
	res->count_type_params = 0;
	res->type_params     = NULL;

	struct Token* next  = list_head(copy);
	
	if(next->kind != TYPEID && next->kind != ANYTYPE){ 
		
		printf("[Parser][Error] Could not parse StructType\n");
		exit(1);
		return NULL; 
	}

	strcpy(res->type_name, next->value_ptr);
	
	list_consume(copy, 1);

	if(list_size(copy) < 1){ goto end; }

	next = list_head(copy);

	if(next->kind == OPKEY_RELATIONAL && strcmp(next->value_ptr, "<")==0 ){
		list_consume(copy, 1);
		parse_type_params_rest(res, copy);
	}
	
	end: 
	list_set(tokens, copy);
	freeTokenListShallow(copy);
	
	return res;
}

static void parse_type_params_rest(struct StructType* res, struct TokenList* tokens){
	
	uint32_t cap = 10;
	
	res->type_params = malloc(sizeof(uint8_t) * cap);
	
	//expect ?TX
	struct Token* next = list_head(tokens);
	
	if(next->kind != TPARAM){
		
		printf("Expected Type Parameter, got:\n");
		list_print(tokens);
		exit(1);
	}
	
	res->type_params[res->count_type_params] = atoi(next->value_ptr);
	res->count_type_params += 1;
	list_consume(tokens, 1);
	
	next = list_head(tokens);
	
	while(next->kind == COMMA){
		
		list_consume(tokens, 1);
		
		if(next->kind != TPARAM){
			printf("Expected Type Parameter, got:\n");
			list_print(tokens);
			exit(1);
		}
		
		res->type_params[res->count_type_params] = atoi(next->value_ptr);
		res->count_type_params += 1;
		list_consume(tokens, 1);
		
		if(res->count_type_params < cap){
			
			next = list_head(tokens);
			continue;
		}
			
		cap *= 2;
		
		const size_t newsize = sizeof(uint8_t) * cap;
		res->type_params = realloc(res->type_params, newsize);

		next = list_head(tokens);
	}
	
	next = list_head(tokens);
	
	//expect '>'
	const bool is_opkey = next->kind == OPKEY_RELATIONAL;
	const bool next_gt = strcmp(next->value_ptr, ">") == 0;
		
	if(is_opkey && next_gt){
		
		list_consume(tokens, 1);
		return;
	}
		
	printf("Syntax Error: expected '>', but got:\n");
	list_print(tokens);
	
	free(res);
	exit(1);
	
	return;
}
