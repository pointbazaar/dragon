#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "Type.h"
#include "ArrayType.h"
#include "TypeParam.h"
#include "BasicType.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/token/token.h"

struct Type* makeType_1(struct BasicType* typeNode){

	struct Type* res = make(Type);
	
	res->super.line_num    = typeNode->super.line_num;
	res->super.annotations = 0;

	res->m1 = typeNode;
	res->m2 = NULL;
	res->m3 = NULL;

	return res;
}

struct Type* makeType_2(struct TypeParam* typeNode){

	struct Type* res = make(Type);
	
	res->super.line_num    = typeNode->super.line_num;
	res->super.annotations = 0;

	res->m1 = NULL;
	res->m2 = typeNode;
	res->m3 = NULL;

	return res;
}

struct Type* makeType_3(struct ArrayType* typeNode){

	struct Type* res = make(Type);
	
	res->super.line_num    = typeNode->super.line_num;
	res->super.annotations = 0;

	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = typeNode;

	return res;
}

struct Type* makeType2(struct TokenList* tokens) {

	struct Type* res = make(Type);
	struct TokenList* copy = list_copy(tokens);
	
	parse_astnode(copy, &(res->super));
	
	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;

	res->m3     = makeArrayType2(copy);
	if(res->m3 != NULL){ goto end; }
		
		
	res->m2     = makeTypeParam(copy);
	if(res->m2 != NULL) { goto end; }
	
	res->m1     = makeBasicType2(copy);
	if(res->m1 != NULL){ goto end; }
	
	//nothing matched
	free(res);
	freeTokenListShallow(copy);
	return NULL;
	
	end:
	
	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}


