#include <stdio.h>
#include <stdlib.h>

#include "Stmt.h"
#include "WhileStmt.h"
#include "ForStmt.h"
#include "LoopStmt.h"
#include "IfStmt.h"
#include "RetStmt.h"
#include "MethodCall.h"
#include "AssignStmt.h"
#include "BreakStmt.h"
#include "SwitchStmt.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../../../token/token.h"
#include "../../../../ast/free_ast.h"

// --- private subroutines ---

struct Stmt* initStmt();

void stmt_make_break(struct Stmt* res, struct TokenList* copy, bool debug);
void stmt_make_loop(struct Stmt* res, struct TokenList* copy, bool debug);
void stmt_make_while(struct Stmt* res, struct TokenList* copy, bool debug);
void stmt_make_if(struct Stmt* res, struct TokenList* copy, bool debug);
void stmt_make_return(struct Stmt* res, struct TokenList* copy, bool debug);
void stmt_make_for(struct Stmt* res, struct TokenList* copy, bool debug);
void stmt_make_other(struct Stmt* res, struct TokenList* copy, bool debug);
void stmt_make_switch(struct Stmt* res, struct TokenList* copy, bool debug);
// ---------------------------

struct Stmt* makeStmt(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Stmt(...)\n");
	}
	
	if (list_size(tokens) == 0) { return NULL; }

	struct Stmt* res = initStmt();

	struct TokenList* copy = list_copy(tokens);

	struct Token* first = list_head(copy);
	
	switch(first->kind){
		case BREAK: stmt_make_break(res, copy, debug); break;
		case FOR: 	stmt_make_for(res, copy, debug); break;
		case LOOP: 	stmt_make_loop(res, copy, debug); break;
		case WHILE: stmt_make_while(res, copy, debug); break;
		case IF: 	stmt_make_if(res, copy, debug); break;
		case RETURN: stmt_make_return(res, copy, debug); break;
		case SWITCH: stmt_make_switch(res, copy, debug); break;
		default: stmt_make_other(res, copy, debug); break;
	}

	if(debug){
		printf("sucess parsing Stmt\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

struct Stmt* initStmt(){
	
	struct Stmt* res = malloc(sizeof(struct Stmt));

	//init
	res->kind = 0;
	res->ptr.m0 = NULL;
	
	return res;
}

void stmt_make_break(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->kind = 6;
	res->ptr.m6 = makeBreakStmt(copy, debug);
	
	if(res->ptr.m6 == NULL){
		printf("expected break stmt, but was:\n");
		list_print(copy);
		freeTokenListShallow(copy);
		free(res);
		exit(1);
	}
}

void stmt_make_loop(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->kind = 0;
	res->ptr.m0 = makeLoopStmt(copy, debug);
		
	if(res->ptr.m0 == NULL){
		printf("expected loop stmt, but was:\n");
		list_print(copy);
		
		freeTokenListShallow(copy);
		free(res);
		
		exit(1);
	}
}

void stmt_make_while(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->kind = 2;
	res->ptr.m2 = makeWhileStmt(copy,debug);
	if(res->ptr.m2 == NULL){
		printf("expected while stmt, but was:\n");
		list_print(copy);
		
		freeTokenListShallow(copy);
		free(res);
		
		exit(1);
	}
}

void stmt_make_if(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->kind = 3;
	res->ptr.m3 = makeIfStmt(copy,debug);
	if(res->ptr.m3 == NULL){
		printf("expected if stmt, but was:\n");
		list_print(copy);
		
		freeTokenListShallow(copy);
		free(res);
		exit(1);
	}
}

void stmt_make_return(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->kind = 4;
	res->ptr.m4 = makeRetStmt(copy,debug);
	if(res->ptr.m4 == NULL){
		printf("expected return stmt, but was:\n");
		list_print(copy);
		
		freeTokenListShallow(copy);
		free(res);
		exit(1);
	}
}

void stmt_make_for(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->kind = 7;
	res->ptr.m7 = makeForStmt(copy,debug);
	if(res->ptr.m7 == NULL){
		printf("expected for stmt, but was:\n");
		list_print(copy);
		
		freeTokenListShallow(copy);
		free(res);
		exit(1);
	}
}

void stmt_make_other(struct Stmt* res, struct TokenList* copy, bool debug){
	
	//TODO: we have to figure something out here.
	//i don't want 'let' statements
	//because it just messes up the code and is
	//just bloat to write.
	//but parsing should be straightforward
	//and mostly deterministic
	//because it gives good performance and
	//to give good error messages
	
	res->kind = 5;
	res->ptr.m5 = makeAssignStmt(copy,debug);
	
	if(res->ptr.m5 == NULL){
		
		res->kind = 1;
		res->ptr.m1 = makeMethodCall(copy,debug);
		if(res->ptr.m1 == NULL){
			printf("expected method call, but was:\n");
			list_print(copy);
			
			freeTokenListShallow(copy);
			free(res);
			
			exit(1);
		}
		if(!list_expect(copy, SEMICOLON)){
			printf("expected ';', but was:\n");
			list_print(copy);
			
			freeTokenListShallow(copy);
			freeMethodCall(res->ptr.m1);
			free(res);
			
			exit(1);
		}
	}
}

void stmt_make_switch(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->kind = 8;
	res->ptr.m8 = makeSwitchStmt(copy,debug);
	if(res->ptr.m8 == NULL){
		printf("expected switch stmt, but was:\n");
		list_print(copy);
		
		freeTokenListShallow(copy);
		free(res);
		exit(1);
	}
}