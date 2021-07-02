#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ast/ast.h"

#include "tables/symtable/symtable.h"
#include "tables/lvst/lvst.h"

#include "typeinference/util/type_str.h"
#include "typeinference/typeinfer.h"

#include "lv_analyzer.h"

#include "ast/visitor/visitor.h"

static void lv_for_stmt   (struct ST* st, struct ForStmt* l);
static void lv_assign_stmt(struct ST* st, struct AssignStmt* a);

static void lv_visitor(void* node, enum NODE_TYPE type, void* arg);

void lvst_fill(struct Method* subr, struct ST* st, bool debug){
	
	for(int i = 0; i < subr->count_args; i++){
		
		struct DeclArg* da = subr->args[i];
		
		char* name = da->name; 
		struct Type* type = da->type;
		
		struct LVSTLine* line = make(LVSTLine);
		
		strncpy(line->name, name, DEFAULT_STR_SIZE);
		line->type = type;
		line->isArg = true;
		line->firstOccur = NULL;
		
		lvst_add(st->lvst, line);
	}
	
	discoverLVStmtBlock(st, subr->block);
	
	if(debug){ lvst_print(st->lvst); }
}

void discoverLVStmtBlock(struct ST* st, struct StmtBlock* b){
	
	visitStmtBlock(b, lv_visitor, st);
}

static void lv_visitor(void* node, enum NODE_TYPE type, void* arg){
	
	struct ST* myst = (struct ST*) arg;

	if(type == NODE_ASSIGNSTMT)
		{ lv_assign_stmt(myst, (struct AssignStmt*) node); }
		
	if(type == NODE_FORSTMT)
		{ lv_for_stmt(myst, (struct ForStmt*) node); }
}

static void lv_for_stmt(struct ST* st, struct ForStmt* l){
	
	struct LVSTLine* line = make(LVSTLine);
	
	strncpy(line->name, l->indexName, DEFAULT_STR_SIZE);
	
	line->type = typeFromStrPrimitive(st, "int");
	
	line->firstOccur = NULL; 
	line->isArg      = false;
	
	lvst_add(st->lvst, line);
	
	discoverLVStmtBlock(st, l->block);
}

static void lv_assign_stmt(struct ST* st, struct AssignStmt* a){
	
	char* varName = a->var->simpleVar->name;
	
	if(lvst_contains(st->lvst, varName)){ return; }
	
	struct LVSTLine* line = make(LVSTLine);
	
	line->firstOccur = a;
	line->isArg      = false;
	
	strncpy(line->name, varName, DEFAULT_STR_SIZE);
	
	if(a->optType != NULL){ 
		
		line->type = a->optType; 
		
	}else{
		
		line->type = infer_type_expr("NO FILENAME", st, a->expr);
	}
	
	lvst_add(st->lvst, line);
}
