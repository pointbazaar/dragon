#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "StructDecl.h"
#include "StructMember.h"
#include "typenodes/SimpleType.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../../../token/token.h"
#include "../../../util/util.h"
#include "../../../ast/free_ast.h"

struct StructDecl* makeStructDecl(struct TokenList* tokens, bool debug){

	if(debug){
		printf("makeStructDecl(...) from: ");
		list_print(tokens);
	}

	struct StructDecl* res = smalloc(sizeof(struct StructDecl));
	
	res->members = smalloc(sizeof(struct StructMember*)*1);
	res->count_members = 0;
	
	struct TokenList* copy = list_copy(tokens);

	if(list_size(copy) > 0){
	
		struct Token* next = list_head(copy);
		
		if(next->kind != STRUCT){
			printf("parsing error, expected 'struct' , but got: %s\n", list_code(copy,debug));
			exit(1);
		}
		list_consume(copy, 1);
		
		
		//read the struct type
		struct SimpleType* st = makeSimpleType2(copy, debug);
		if(st == NULL){
			printf("expected SimpleType, but got: \n");
			list_print(copy);
			exit(1);
		}
		res->type = st;
		
		
			
		next = list_head(copy);
		if(next->kind != LCURLY){
			printf("parsing error, expected '{' , but got: %s\n", list_code(copy,debug));
			exit(1);
		}
		list_consume(copy, 1);
		
		struct StructMember* member;
		
		member = makeStructMember(copy, debug);
		while(member != NULL){
			
			res->members[res->count_members] = member;
			res->count_members++;
			
			next = list_head(copy);
			if(next->kind != COMMA){
				break;
			}
			list_consume(copy, 1);
			
			//we just hope that the realloc will go through here
			res->members = realloc(
				res->members, 
				sizeof(struct StructMember*)*(res->count_members+1)
			);
			
			member = makeStructMember(copy, debug);
		}
		
		next = list_head(copy);
		if(next->kind != RCURLY){
			printf("parsing error, expected '}' , but got: %s\n", list_code(copy,debug));
			exit(1);
		}
		list_consume(copy, 1);
	}
	
	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

