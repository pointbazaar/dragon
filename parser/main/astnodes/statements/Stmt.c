#include <stdio.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "Stmt.h"
#include "WhileStmt.h"
#include "ForStmt.h"
#include "LoopStmt.h"
#include "IfStmt.h"
#include "RetStmt.h"
#include "Call.h"
#include "AssignStmt.h"
#include "SwitchStmt.h"
#include "TryCatchStmt.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

// --- private subroutines ---

static struct Stmt* initStmt();

static void stmt_make_loop(struct Stmt* res, struct TokenList* copy, bool debug);
static void stmt_make_while(struct Stmt* res, struct TokenList* copy, bool debug);
static void stmt_make_if(struct Stmt* res, struct TokenList* copy, bool debug);
static void stmt_make_return(struct Stmt* res, struct TokenList* copy, bool debug);
static void stmt_make_for(struct Stmt* res, struct TokenList* copy, bool debug);
static void stmt_make_other(struct Stmt* res, struct TokenList* copy, bool debug);
static void stmt_make_switch(struct Stmt* res, struct TokenList* copy, bool debug);
static void stmt_make_trycatch(struct Stmt* res, struct TokenList* copy, bool debug);
// ---------------------------

struct Stmt* makeStmt(struct TokenList* tokens) {
	
	if (list_size(tokens) == 0) { return NULL; }

	struct Stmt* res = initStmt();
	struct TokenList* copy = list_copy(tokens);
	
	parse_astnode(copy, &(res->super));

	res->kind 	    = -1;
	res->is_break 	= false;
	res->is_continue = false;
	res->is_throw    = false;

	struct Token* first = list_head(copy);
	
	switch(first->kind){

		case BREAK:	
			res->kind = 99;  res->is_break = true;
			list_consume(copy, 2); 
			break;

		case CONTINUE:	
			res->kind = 99;  res->is_continue = true;
			list_consume(copy, 2); 
			break;
			
		case THROW:
			res->kind = 99;  res->is_throw = true;
			list_consume(copy, 2); 
			break;

		case FOR:       stmt_make_for(res, copy, false);    break;
		case LOOP:      stmt_make_loop(res, copy, false);   break;
		case WHILE:     stmt_make_while(res, copy, false); 	break;
		case IF:        stmt_make_if(res, copy, false);     break;
		case RETURN:    stmt_make_return(res, copy, false); break;
		case SWITCH:    stmt_make_switch(res, copy, false); break;
		case TRY:		stmt_make_trycatch(res, copy, false); break;

		default: 		stmt_make_other(res, copy, false); 	break;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

struct Stmt* initStmt(){
	
	struct Stmt* res = make(Stmt);

	//init
	res->kind = 0;
	res->ptr.m0 = NULL;
	
	return res;
}

void stmt_make_loop(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->kind = 0;
	res->ptr.m0 = makeLoopStmt(copy);
		
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
	res->ptr.m2 = makeWhileStmt(copy);
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
	res->ptr.m3 = makeIfStmt(copy);
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
	res->ptr.m4 = makeRetStmt(copy);
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
	res->ptr.m7 = makeForStmt(copy);
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
	res->ptr.m5 = makeAssignStmt(copy);
	
	if(res->ptr.m5 == NULL){
		
		res->kind = 1;
		res->ptr.m1 = makeCall(copy);
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
			free_call(res->ptr.m1);
			free(res);
			
			exit(1);
		}
	}
}

void stmt_make_switch(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->kind = 8;
	res->ptr.m8 = makeSwitchStmt(copy);
	if(res->ptr.m8 == NULL){
		printf("expected switch stmt, but was:\n");
		list_print(copy);
		
		freeTokenListShallow(copy);
		free(res);
		exit(1);
	}
}

static void stmt_make_trycatch(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->kind = 6;
	res->ptr.m6 = makeTryCatchStmt(copy);
	if(res->ptr.m8 == NULL){
		printf("expected try-catch stmt, but was:\n");
		list_print(copy);
		
		freeTokenListShallow(copy);
		free(res);
		exit(1);
	}
}
