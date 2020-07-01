#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Namespace.h"
#include "Method.h"
#include "StructDecl.h"
#include "../commandline/TokenList.h"
#include "../commandline/Token.h"
#include "../commandline/TokenKeys.h"

struct Namespace* makeNamespace(struct TokenList* tokens, char* name, bool debug) {

	if (debug) {
		printf("Namespace(...) from: %s\n", list_code(tokens));
	}

	struct Namespace* res = malloc(sizeof(struct Namespace));

	if(res==NULL){
		printf("could not malloc.\n");
		exit(1);
	}

	res->count_methods = 0;
	res->count_structs = 0;

	res->methods = malloc(sizeof(struct Method*)*1);
	res->structs = malloc(sizeof(struct StructDecl*)*1);

	res->srcPath = "/dev/null";
	res->name = name;
	struct TokenList* copy_1 = list_copy(tokens);
	struct TokenList* copy = copy_1;

	
	if(list_size(copy)>0) {

		struct Token* next_struct = list_get(copy, 0);
		if(next_struct == NULL){return NULL;}

		while (next_struct->kind == STRUCT) {
			struct StructDecl* mystructdecl = makeStructDecl(copy,debug);
			if(mystructdecl == NULL){return NULL;}

			res->structs[res->count_structs++] = mystructdecl;
			res->structs = realloc(res->structs,sizeof(struct StructDecl*)*(res->count_structs+1));

			if (list_size(copy) > 0) {
				next_struct = list_get(copy, 0);
				if(next_struct == NULL){return NULL;}
			} else {
				break;
			}
		}
	}
	

	//it is be nice to have prefix 'fn' before a function
	//to make parsing easier.
	//this does not add much boilerplate to the syntax
	//and would probably make the parser faster
	
	if (list_size(copy) > 0) {

		struct Token* next_subr = list_get(copy, 0);

		while (next_subr->kind == FN) {
			struct Method* mthd = makeMethod(copy,debug);
			if(mthd == NULL){return NULL;}

			res->methods[res->count_methods++] = mthd;
			res->methods = realloc(res->methods,sizeof(struct Method*)*(res->count_methods+1));

			if (list_size(copy) > 0) {
				next_subr = list_get(copy, 0);
			} else {
				break;
			}
		}
	}

	if(debug){
		printf("done parsing Namespace Node\n");
	}

	list_set(tokens, copy);

	return res;
}

