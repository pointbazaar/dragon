#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"

#include "tables/symtable.h"
#include "tables/lvst.h"

#include "typeinference/util/type_str.h"
#include "typeinference/typeinfer.h"

#include "analyzer/lv_analyzer.h"

static void discoverLVStmt      (struct ST* st, struct Stmt* s);
static void discoverLVIfStmt    (struct ST* st, struct IfStmt* i);
static void discoverLVWhileStmt (struct ST* st, struct WhileStmt* w);
static void discoverLVLoopStmt  (struct ST* st, struct LoopStmt* l);
static void discoverLVForStmt   (struct ST* st, struct ForStmt* l);
static void discoverLVAssignStmt(struct ST* st, struct AssignStmt* a);
static void discoverLVSwitchStmt(struct ST* st, struct SwitchStmt* a);
static void discoverLVCaseStmt  (struct ST* st, struct CaseStmt* c);

void discoverLVStmtBlock(struct ST* st, struct StmtBlock* b){
	
	for(int i = 0; i < b->count; i++)
		{ discoverLVStmt(st, b->stmts[i]); }
}

static void discoverLVStmt(struct ST* st, struct Stmt* s){
	
	switch(s->kind){
		
		case 0: discoverLVLoopStmt  (st, s->ptr.m0); break;
		case 2: discoverLVWhileStmt (st, s->ptr.m2); break;
		case 3: discoverLVIfStmt    (st, s->ptr.m3); break;
		case 5: discoverLVAssignStmt(st, s->ptr.m5); break;
		case 7: discoverLVForStmt   (st, s->ptr.m7); break;
		case 8: discoverLVSwitchStmt(st, s->ptr.m8); break;
		
		default: break;
	}
}

static void discoverLVIfStmt(struct ST* st, struct IfStmt* i){
	
	discoverLVStmtBlock(st, i->block);
	
	if(i->elseBlock != NULL)
		{ discoverLVStmtBlock(st, i->elseBlock); }
}

static void discoverLVWhileStmt(struct ST* st, struct WhileStmt* w){
	
	discoverLVStmtBlock(st, w->block);
}

static void discoverLVLoopStmt(struct ST* st, struct LoopStmt* l){
	
	discoverLVStmtBlock(st, l->block);
}

static void discoverLVForStmt(struct ST* st, struct ForStmt* l){
	
	struct LVSTLine* line = make(LVSTLine);
	
	strncpy(line->name, l->indexName, DEFAULT_STR_SIZE);
	
	line->type = typeFromStrPrimitive(st, "int");
	
	line->firstOccur = NULL; 
	line->isArg = false;
	
	lvst_add(st->lvst, line);
	
	discoverLVStmtBlock(st, l->block);
}

static void discoverLVAssignStmt(struct ST* st, struct AssignStmt* a){
	
	char* varName = a->var->simpleVar->name;
	
	if(lvst_contains(st->lvst, varName)){ return; }
	
	struct LVSTLine* line = make(LVSTLine);
	
	line->isArg = false;
	line->firstOccur = a;
	
	strncpy(line->name, varName, DEFAULT_STR_SIZE);
	
	if(a->optType != NULL){ 
		
		line->type = a->optType; 
		
	}else{
		
		line->type = infer_type_expr(st, a->expr);
	}
	
	lvst_add(st->lvst, line);
}

static void discoverLVSwitchStmt(struct ST* st, struct SwitchStmt* s){
	
	for(int i=0; i < s->count_cases; i++)
		{ discoverLVCaseStmt(st, s->cases[i]); }
}

static void discoverLVCaseStmt(struct ST* st, struct CaseStmt* c){
	
	if(c->block != NULL)
		{ discoverLVStmtBlock(st, c->block); }
}