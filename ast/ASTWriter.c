#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ASTWriter.h"
#include "../util/util.h"

// --- private subroutines (private to this file) ---

// --- high level structures ---------

void writeNamespace(struct Namespace* m, 	FILE* file);

void writeMethod(struct Method* m, 		FILE* file);

void writeStructDecl(struct StructDecl* m, FILE* file);
void writeStructMember(struct StructMember* m, FILE* file);

void writeStmtBlock(struct StmtBlock* block, FILE* file);

// -----------------------------------

void writeDeclArg(struct DeclArg* m, 		FILE* file);
void writeExpr(struct Expr* m, 			FILE* file);
void writeOp(struct Op* m, 			FILE* file);

//const nodes
void writeIntConst(struct IntConst* m, 		 FILE* file);
void writeBoolConst(struct BoolConst* m, 	 FILE* file);
void writeCharConst(struct CharConst* m, 	 FILE* file);
void writeFloatConst(struct FloatConst* m, 	 FILE* file);
void writeStringConst(struct StringConst* m, FILE* file);

void writeVariable(struct Variable* m, 		FILE* file);
void writeSimpleVar(struct SimpleVar* m, 	FILE* file);
void writeTerm(struct Term* m, 				FILE* file);
void writeUnOpTerm(struct UnOpTerm* t,  	FILE* file);
void writeRange(struct Range* r, 			FILE* file);

//statementnodes
void writeStmt(struct Stmt* m, 			FILE* file);
void writeIfStmt(struct IfStmt* m, 		FILE* file);
void writeWhileStmt(struct WhileStmt* m, 	FILE* file);
void writeRetStmt(struct RetStmt* m, 		FILE* file);
void writeAssignStmt(struct AssignStmt* m, 	FILE* file);
void writeMethodCall(struct MethodCall* m, 	FILE* file);
void writeLoopStmt(struct LoopStmt* m, 		FILE* file);
void writeBreakStmt(struct BreakStmt* m, 	FILE* file);
void writeForStmt(struct ForStmt* m,	 	FILE* file);
void writeSwitchStmt(struct SwitchStmt* m,	FILE* file);
void writeCaseStmt(struct CaseStmt* m,		FILE* file);

//typenodes
void writeType(struct Type* m, 				FILE* file);
void writeSubrType(struct SubrType* m, 			FILE* file);
void writeSimpleType(struct SimpleType* m, 		FILE* file);
void writeArrayType(struct ArrayType* m, 		FILE* file);
void writeTypeParam(struct TypeParam* m, 		FILE* file);
void writeBasicTypeWrapped(struct BasicTypeWrapped* m, 	FILE* file);


// ---------------------------

void writeNamespace(struct Namespace* nsn, FILE* file){
	
	fprintf(file,
		"%s\t%s\t",
		nsn->srcPath,
		nsn->name
	);
	
	//make a newline for easier reading
	fprintf(file, "\n");
	
	fprintf(file, "%d\t", nsn->count_methods);
	fprintf(file, "\n");
	
	//write methods
	for(int i=0;i < nsn->count_methods;i++){ 
		struct Method* m = nsn->methods[i];
		writeMethod(m,file);
		
		//make a newline for easier reading
		fprintf(file, "\n");
	}
	
	fprintf(file, "%d\t", nsn->count_structs);
	fprintf(file, "\n");
	
	//write structs
	for(int i=0;i < nsn->count_structs;i++){ 
		struct StructDecl* m = nsn->structs[i];
		writeStructDecl(m,file);
		
		//make a newline for easier reading
		fprintf(file, "\n");
	}
	
}
void writeMethod(struct Method* m, FILE* file){
	
	fprintf(file, "Method\t");

	fprintf(file,
		"%d\t%d\t%s\t",
		m->isPublic,
		m->hasSideEffects,
		m->name
	);
	fprintf(file, "\n");
	writeType(m->returnType,file);

	//write arguments
	fprintf(file, "\n");
	
	fprintf(file,"%d\t",m->count_args);
	for(int i = 0;i < m->count_args;i++){ 
		struct DeclArg* arg = m->args[i];
		writeDeclArg(arg, file); 
	}

	fprintf(file, "\n");
	writeStmtBlock(m->block, file);
}
void writeStructDecl(struct StructDecl* m, FILE* file){
	
	fprintf(file, "StructDecl\t");
	fprintf(file, "%s\t%d\t", m->name, m->count_members);
	for(int i=0;i < m->count_members;i++){
		writeStructMember(m->members[i], file);
	}
}
void writeStructMember(struct StructMember* m, FILE* file){
	
	fprintf(file, "StructMember\t");
	writeType(m->type, file);
	fprintf(file, "%s\t", m->name);
}

void writeStmtBlock(struct StmtBlock* block, FILE* file){
	
	fprintf(file, "StmtBlock\t");
	fprintf(file, "%d\t", block->count);
	for(int i=0;i < block->count; i++){
		writeStmt(block->stmts[i], file);
	}
}

// ----------------------------

void writeDeclArg(struct DeclArg* m, FILE* file){
	
	fprintf(file, "DeclaredArg\t");

	writeType(m->type,file);
	
	int option = (m->name != NULL)?1:0;
	
	fprintf(file, "%d\t", option);
	
	if(m->name != NULL){
		fprintf(file,"%s\t",m->name);
	}
}
void writeVariable(struct Variable* m, FILE* file){

	fprintf(file, "Variable\t");

	writeSimpleVar(m->simpleVar,file);

	fprintf(file, "%d\n", m->count_memberAccessList);
	for(int i = 0;i < m->count_memberAccessList;i++){ 
		struct Variable* v = m->memberAccessList[i];
		writeVariable(v, file); 
	}
}
void writeSimpleVar(struct SimpleVar* m, FILE* file){

	fprintf(file, "SimpleVar\t%s\t",m->name);
	
	fprintf(file, "%d\t", m->count_indices);
	
	for(int i=0;i < m->count_indices; i++){
		writeExpr(m->indices[i], file);
	}
}
void writeExpr(struct Expr* m, FILE* file){

	fprintf(file, "Expr\t");

	writeUnOpTerm(m->term1, file);
	
	if(m->op != NULL){
		fprintf(file, "0\t");
		
		writeOp(m->op,file);
		writeUnOpTerm(m->term2,file);
	}else{
		fprintf(file,"1\t");
	}
}
void writeTerm(struct Term* m, FILE* file){

	fprintf(file, "Term\t");

	fprintf(file,"%d\t", m->kind);

	switch(m->kind){
		case 1:
			writeBoolConst(m->ptr.m1,file);
			break;
		case 2:
			writeIntConst(m->ptr.m2,file); 
			break;
		case 3:
			writeCharConst(m->ptr.m3,file); 
			break;
		case 4:
			writeMethodCall(m->ptr.m4,file); 
			break;
		case 5:
			writeExpr(m->ptr.m5,file); 
			break;
		case 6:
			writeVariable(m->ptr.m6,file); 
			break;
		case 7:
			writeFloatConst(m->ptr.m7,file); 
			break;
		case 8:
			writeStringConst(m->ptr.m8, file);
			break;
		default:
			printf("Error in writeTerm(...)\n");
			exit(1);
	}
}
void writeUnOpTerm(struct UnOpTerm* t,  	FILE* file){
	fprintf(file, "UnOpTerm\t");
	
	fprintf(file, "%d\t", (t->op == NULL)?0:1);
	
	if(t->op != NULL){
		writeOp(t->op, file);
	}
	
	writeTerm(t->term, file);
}
void writeRange(struct Range* r, FILE* file){
	
	fprintf(file, "Range\t");
	
	writeExpr(r->start, file);
	writeExpr(r->end, file);
}
// --------- CONST NODES ----------------
void writeBoolConst(struct BoolConst* m, FILE* file){
	fprintf(file, "BoolConst\t%d\t", m->value);
}
void writeIntConst(struct IntConst* m, FILE* file){
	fprintf(file, "IntConst\t%d\t", m->value);
}
void writeCharConst(struct CharConst* m, FILE* file){
	fprintf(file, "CharConst\t%c\t", m->value);
}
void writeFloatConst(struct FloatConst* m, 	FILE* file){
	fprintf(file, "FloatConst\t%f\t", m->value);
}
void writeStringConst(struct StringConst* m, FILE* file){
	fprintf(file, "StringConst\t");
	
	//print the length
	int l = strlen(m->value);
	fprintf(file, "%d\t", l);
	
	//doing this to avoid problems
	//with whitespace or any characters at all really
	for(int i=0;i < l; i++){
		//0-padded on the left, 3 chars wide,
		//casted to unsigned integer
		fprintf(file, "%03d_", (unsigned int)m->value[i]);
	}
	//separator
	fprintf(file, "\t");
}
void writeOp(struct Op* m, FILE* file){
	fprintf(file, "Op\t%s\t", m->op);
}
// ---------------- STATEMENTNODES ---------------------
void writeStmt(struct Stmt* m, FILE* file){

	//to make it more human-readable / debuggable
	fprintf(file, "Stmt\t");

	//the reader has to know which type it is,
	//we can print a small number
	fprintf(file,"%d\t", m->kind);
	
	switch(m->kind){
		case 0: { writeLoopStmt(m->ptr.m0, file);  } break;
		case 1: { writeMethodCall(m->ptr.m1,file); } break;
		case 2: { writeWhileStmt(m->ptr.m2,file);  } break;
		case 3: { writeIfStmt(m->ptr.m3,file);     } break;
		case 4: { writeRetStmt(m->ptr.m4,file);    } break;
		case 5: { writeAssignStmt(m->ptr.m5,file); } break;
		case 6: { writeBreakStmt(m->ptr.m6,file);  } break;
		case 7: { writeForStmt(m->ptr.m7,file);    } break;
		case 8: { writeSwitchStmt(m->ptr.m8,file); } break;
	}
}

void writeIfStmt(struct IfStmt* m, FILE* file){

	fprintf(file, "IfStmt\t");

	writeExpr(m->condition, file);

	writeStmtBlock(m->block, file);
	
	//indicate if there is an else block
	fprintf(file, "%d\t", (m->elseBlock != NULL)?1:0);
	
	if(m->elseBlock != NULL){
		writeStmtBlock(m->elseBlock, file);
	}
}

void writeWhileStmt(struct WhileStmt* m, FILE* file){

	fprintf(file, "WhileStmt\t");

	writeExpr(m->condition, file);

	writeStmtBlock(m->block, file);
}

void writeRetStmt(struct RetStmt* m, FILE* file){

	fprintf(file, "RetStmt\t");

	writeExpr(m->returnValue,file);
}

void writeAssignStmt(struct AssignStmt* m, FILE* file){

	fprintf(file, "AssignStmt\t");
	
	int option = (m->optType != NULL)?1:0;
	
	fprintf(file, "%d\t", option);

	if(m->optType != NULL){
		writeType(m->optType, file);
	}
	
	writeVariable(m->var, file);
	
	fprintf(file, "%s\t", m->assign_op);
	
	writeExpr(m->expr, file);
}
void writeMethodCall(struct MethodCall* m, FILE* file){

	fprintf(file, "MethodCall\t");

	fprintf(file, "%s\t", m->methodName);
	fprintf(file, "%d\t", m->count_args);
	for(int i=0;i < m->count_args;i++){ 
		struct Expr* e = m->args[i];
		writeExpr(e,file); 
	}
}
void writeLoopStmt(struct LoopStmt* m, FILE* file){
	
	fprintf(file, "LoopStmt\t");

	writeExpr(m->count, file);

	writeStmtBlock(m->block, file);
}
void writeBreakStmt(struct BreakStmt* m, FILE* file){
	
	fprintf(file, "BreakStmt\t");
}
void writeForStmt(struct ForStmt* m, FILE* file){
	
	fprintf(file, "ForStmt\t%s\t", m->indexName);

	writeRange(m->range, file);

	writeStmtBlock(m->block, file);
}
void writeSwitchStmt(struct SwitchStmt* m,	FILE* file){
	
	fprintf(file, "SwitchStmt\t");
	
	writeVariable(m->var, file);
	
	fprintf(file, "%d\t", m->count_cases);
	
	for(int i=0; i < m->count_cases; i++){
		
		writeCaseStmt(m->cases[i], file);
	}
}
void writeCaseStmt(struct CaseStmt* m,		FILE* file){
	
	fprintf(file, "CaseStmt\t%d\t", m->kind);
	
	switch(m->kind){
		case 0: writeBoolConst(m->ptr.m1, file); break;
		case 1: writeCharConst(m->ptr.m2, file); break;
		case 2: writeIntConst(m->ptr.m3, file); break;
		default: 
			printf("Error\n");
			fclose(file);
			exit(1);
	}
	
	fprintf(file, "%d\t", (m->block == NULL)?0:1);
	
	if(m->block != NULL){
		
		writeStmtBlock(m->block, file);
	}
}
// --------- TYPENODES --------------
void writeType(struct Type* m, FILE* file){
	//there is an alternative. we give a small number to indicate the alternative
	fprintf(file, "Type\t");
	
	if(m->m1 != NULL){
		fprintf(file,"1\t");
		writeBasicTypeWrapped(m->m1,file);

	}else if(m->m2 != NULL){
		fprintf(file,"2\t");
		writeTypeParam(m->m2,file);

	}else if(m->m3 != NULL){
		fprintf(file,"3\t");
		writeArrayType(m->m3,file);
	}
}

void writeArrayType(struct ArrayType* m, FILE* file){
	fprintf(file, "ArrayType\t");
	writeType(m->element_type,file);
}

void writeTypeParam(struct TypeParam* m, FILE* file){
	fprintf(file, "TypeParam\t");
	fprintf(file,"%d\t",m->index);
}

void writeBasicTypeWrapped(struct BasicTypeWrapped* m, FILE* file){
	fprintf(file, "BasicTypeWrapped\t");

	if(m->simpleType != NULL){ 
		fprintf(file,"1\t"); 
		writeSimpleType(m->simpleType,file); 
	}else if(m->subrType != NULL){ 
		fprintf(file,"2\t"); 
		writeSubrType(m->subrType,file); 
	}
}

void writeSimpleType(struct SimpleType* m, FILE* file){
	fprintf(file, "SimpleType\t");
	fprintf(file,"%s\t",m->typeName);
}

void writeSubrType(struct SubrType* m, FILE* file){
	fprintf(file, "SubrType\t");

	writeType(m->returnType,file);
	fprintf(file,"%d\t",m->hasSideEffects);

	fprintf(file,"%d\t",m->count_argTypes);
	for(int i = 0;i < m->count_argTypes;i++){
		struct Type* t = m->argTypes[i];
		writeType(t,file); 
	}
}
// --------- OTHER ----------
void write_ast(char* filename, struct Namespace* namespaceNode){
	
	//DEBUG
	//printf("write_ast\n");
	
	FILE* file = fopen(filename, "w");

	if(file == NULL){
		printf("could not open file: %s (in write_ast)\n", filename);
		exit(1);
	}
	
	writeNamespace(namespaceNode, file);
	
	fclose(file);
}
