#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../ast.h"
#include "ast_reader.h"
#include "magic_num.h"
#include "serialize.h"

struct AST* readAST(char** filenames, int count_filenames, bool debug){
	
	if(debug){ printf("readAST(...)\n"); }

	struct AST* ast = make(AST);
	
	ast->count_namespaces = count_filenames;
	ast->namespaces = malloc(sizeof(struct Namespace*)* ast->count_namespaces);

	for(int i=0;i < count_filenames; i++){
		
		char* filename = filenames[i];

		FILE* file = fopen(filename, "r");
		
		if(file == NULL){
			printf("could not open file: %s, in ast_reader.c:readAST\n", filename);
			return NULL;
		}

		struct Namespace* ns = readNamespace(file, debug);
		
		if(ns == NULL){ return NULL; }
		
		ast->namespaces[i] = ns;

		fclose(file);
	}
	
	if(debug){ printf("readAST ~ done\n"); }
	
	return ast;
}

struct Namespace* readNamespace(FILE* file, bool debug){
	
	if(debug){ printf("readNamespace(...)\n"); }
	
	magic_num_require(MAGIC_NAMESPACE, file);
	
	struct Namespace* ns = make(Namespace);

	char* tmpSrcPath = deserialize_string(file);
	strcpy(ns->srcPath, tmpSrcPath);
	free(tmpSrcPath);
	
	char* tmpASTFilename = deserialize_string(file);
	strcpy(ns->ast_filename, tmpASTFilename);
	free(tmpASTFilename);
	
	tmpSrcPath = deserialize_string(file);
	strcpy(ns->name, tmpSrcPath);
	free(tmpSrcPath);
	
	ns->count_methods = deserialize_int(file);

	//read methods
	ns->methods = malloc(sizeof(struct Method*)*(ns->count_methods));
	
	for(int i=0; i < ns->count_methods; i++){
		
		ns->methods[i] = readMethod(file, debug);
	}
	
	//read structs
	ns->count_structs = deserialize_int(file);
	
	ns->structs = 
		malloc(sizeof(struct StructDecl*)*(ns->count_structs));
	
	for(int i=0;i < ns->count_structs; i++){
		
		ns->structs[i] = readStructDecl(file, debug);
	}
	
	magic_num_require(MAGIC_END_NAMESPACE, file);
	
	return ns;
}
struct Method* readMethod(FILE* file, bool debug){
	
	if(debug){ printf("readMethod(...)\n"); }
	
	magic_num_require(MAGIC_METHOD, file);
	
	struct Method* m = make(Method);

	m->isPublic = deserialize_int(file);
	m->hasSideEffects = deserialize_int(file);

	char* tmp = deserialize_string(file);
	strcpy(m->name, tmp);
	
	free(tmp);

	m->returnType = readType(file, debug);
	
	m->count_args = deserialize_int(file);
	
	m->args = malloc(sizeof(struct DeclArg*)*(m->count_args));

	for(int i = 0;i < m->count_args;i++){
		m->args[i] = readDeclArg(file, debug);
	}

	m->block = readStmtBlock(file, debug);
	
	magic_num_require(MAGIC_END_METHOD, file);
	
	return m;
}
struct StructDecl* readStructDecl(FILE* file, bool debug){
	
	if(debug){ printf("readStructDecl(...)\n"); }
	
	magic_num_require(MAGIC_STRUCTDECL, file);
	
	struct StructDecl* res = make(StructDecl);
	
	res->type = readSimpleType(file, debug);
	
	res->count_members = deserialize_int(file);
	
	res->members = 
		malloc(sizeof(struct StructMember*)*res->count_members);
	for(int i=0;i < res->count_members;i++){
		res->members[i] = readStructMember(file, debug);
	}
	
	magic_num_require(MAGIC_END_STRUCTDECL, file);
	
	return res;
}
struct StructMember* readStructMember(FILE* file, bool debug){
	
	if(debug){ printf("readStructMember(...)\n"); }
	
	magic_num_require(MAGIC_STRUCTMEMBER, file);
	
	struct StructMember* res = make(StructMember);
	
	res->type = readType(file, debug);
	
	char* tmp = deserialize_string(file);
	
	strcpy(res->name, tmp);
	free(tmp);
	
	magic_num_require(MAGIC_END_STRUCTMEMBER, file);
	
	return res;
}

struct StmtBlock* readStmtBlock(FILE* file, bool debug){
	
	if(debug){ printf("readStmtBlock(...)\n"); }
	
	magic_num_require(MAGIC_STMTBLOCK, file);
	
	struct StmtBlock* block = make(StmtBlock);
	
	block->count = deserialize_int(file);
	
	block->stmts = 
		malloc(sizeof(struct Stmt*)* block->count);
	
	for(int i=0;i < block->count; i++){
		block->stmts[i] = readStmt(file, debug);
	}
	
	magic_num_require(MAGIC_END_STMTBLOCK, file);
	
	return block;
}

// -----------------------

struct DeclArg* readDeclArg(FILE* file, bool debug){
	
	if(debug){ printf("readDeclArg(...)\n"); }
	
	magic_num_require(MAGIC_DECLARG, file);
	
	struct DeclArg* da = make(DeclArg);

	da->type = readType(file, debug);

	int option = deserialize_int(file);

	da->has_name = option == OPT_PRESENT;

	if(option != 0 && option != 1){

		printf("Error in readDeclArg\n");
		free(da);
		fclose(file);
		exit(1);
	}

	if(da->has_name){
		char* tmp = deserialize_string(file);
		strcpy(da->name, tmp);
		free(tmp);
	}
	
	magic_num_require(MAGIC_END_DECLARG, file);

	return da;
}
struct Expr* readExpr(FILE* file, bool debug){
	
	if(debug){ printf("readExpr(...)\n"); }
	
	magic_num_require(MAGIC_EXPR, file);
	
	struct Expr* expr = make(Expr);

	expr->term1 = readUnOpTerm(file, debug);
	expr->op = NULL;
	expr->term2 = NULL;
	
	const int option = deserialize_int(file);

	if(option != OPT_EMPTY && option != OPT_PRESENT){
		printf("Error in readExpr!\n");
		fclose(file);
		exit(1);
	}
	
	if (option == OPT_PRESENT){
		expr->op = readOp(file, debug);
		expr->term2 = readUnOpTerm(file, debug);
	}
	
	magic_num_require(MAGIC_END_EXPR, file);

	return expr;
}
struct Op* readOp(FILE* file, bool debug){
	
	if(debug){ printf("readOp(...)\n"); }
	
	magic_num_require(MAGIC_OP, file);
	
	struct Op* op = make(Op);
	
	fread(op, sizeof(struct Op), 1, file);
	
	magic_num_require(MAGIC_END_OP, file);

	return op;
}
struct IntConst* readIntConst(FILE* file, bool debug){
	
	if(debug){printf("readIntConst(...)\n");}
	
	magic_num_require(MAGIC_INTCONST, file);
	
	struct IntConst* ic = make(IntConst);
		
	fread(ic, sizeof(struct IntConst), 1, file);
	
	magic_num_require(MAGIC_END_INTCONST, file);

	return ic;
}

struct HexConst* readHexConst(FILE* file, bool debug){
	
	if(debug){printf("readHexConst(...)\n");}
	
	magic_num_require(MAGIC_HEXCONST, file);
	
	struct HexConst* hc = make(HexConst);
	
	fread(hc, sizeof(struct HexConst), 1, file);
	
	magic_num_require(MAGIC_END_HEXCONST, file);

	return hc;
}

struct BinConst* readBinConst(FILE* file, bool debug){
	
	if(debug){printf("readBinConst(...)\n");}
	
	magic_num_require(MAGIC_BINCONST, file);
	
	struct BinConst* hc = make(BinConst);
	
	fread(hc, sizeof(struct BinConst), 1, file);
	
	magic_num_require(MAGIC_END_BINCONST, file);

	return hc;
}

struct BoolConst* readBoolConst(FILE* file, bool debug){
	
	if(debug){ printf("readBoolConst(...)\n"); }
	
	magic_num_require(MAGIC_BOOLCONST, file);
	
	struct BoolConst* b = make(BoolConst);
	
	fread(b, sizeof(struct BoolConst), 1, file);
	
	magic_num_require(MAGIC_END_BOOLCONST, file);
	
	return b;
}
struct CharConst* readCharConst(FILE* file, bool debug){
	
	if(debug){ printf("readCharConst(...)\n"); }
	
	magic_num_require(MAGIC_CHARCONST, file);
	
	struct CharConst* b = make(CharConst);
	
	fread(b, sizeof(struct CharConst), 1, file);
	
	magic_num_require(MAGIC_END_CHARCONST, file);
	
	return b;
}
struct FloatConst* readFloatConst(FILE* file, bool debug){
	
	if(debug){ printf("readFloatConst(...)\n"); }
	
	magic_num_require(MAGIC_FLOATCONST, file);
	
	struct FloatConst* ic = make(FloatConst);
		
	fread(ic, sizeof(struct FloatConst), 1, file);
	
	magic_num_require(MAGIC_END_FLOATCONST, file);
	
	return ic;
}
struct StringConst* readStringConst(FILE* file, bool debug){
	
	if(debug){ printf("readStringConst(...)\n"); }
	
	magic_num_require(MAGIC_STRINGCONST, file);
	
	struct StringConst* s = make(StringConst);
	
	//doing this to avoid problems
	//with whitespace or any characters at all really
	s->value = deserialize_string(file);
	
	magic_num_require(MAGIC_END_STRINGCONST, file);
	
	return s;
}
struct Variable* readVariable(FILE* file, bool debug){
	
	if(debug){ printf("readVariable(...)\n"); }
	
	magic_num_require(MAGIC_VARIABLE, file);
	
	struct Variable* v = make(Variable);
	v->memberAccess = NULL;
	v->simpleVar = readSimpleVar(file, debug);

	const bool hasMemberAccess = deserialize_int(file) == OPT_PRESENT;
	
	if(hasMemberAccess){
		v->memberAccess = readVariable(file, debug);
	}

	magic_num_require(MAGIC_END_VARIABLE, file);
	
	return v;
}
struct SimpleVar* readSimpleVar(FILE* file, bool debug){
	
	if(debug){ printf("readSimpleVar(...)\n"); }
	
	magic_num_require(MAGIC_SIMPLEVAR, file);
	
	struct SimpleVar* b = make(SimpleVar);
	
	char* tmp = deserialize_string(file);
	strcpy(b->name, tmp);
	free(tmp);

	b->count_indices = deserialize_int(file);
	
	b->indices = malloc(sizeof(struct Expr*)* (b->count_indices+1));
	
	for(int i=0; i < b->count_indices; i++){
		b->indices[i] = readExpr(file, debug);
	}
	
	magic_num_require(MAGIC_END_SIMPLEVAR, file);
	
	return b;
}
struct Term* readTerm(FILE* file, bool debug){
	
	if(debug){ printf("readTerm(...)\n"); }
	
	magic_num_require(MAGIC_TERM, file);
	
	struct Term* b = make(Term);
	
	b->kind = deserialize_int(file);

	switch(b->kind){
	
		case  1: b->ptr.m1  = readBoolConst(file, debug); 	break;
		case  2: b->ptr.m2  = readIntConst(file, debug); 	break;
		case  3: b->ptr.m3  = readCharConst(file, debug); 	break;
		case  4: b->ptr.m4  = readMethodCall(file, debug); 	break;
		case  5: b->ptr.m5  = readExpr(file, debug); 		break;
		case  6: b->ptr.m6  = readVariable(file, debug); 	break;
		case  7: b->ptr.m7  = readFloatConst(file, debug); 	break;
		case  8: b->ptr.m8  = readStringConst(file, debug); break;
		case  9: b->ptr.m9  = readHexConst(file, debug); 	break;
		case 10: b->ptr.m10 = readBinConst(file, debug); 	break;
		
		default:
			printf("Error in readTerm\n");
			free(b);
			fclose(file);
			exit(1);
			break;
	}
	
	magic_num_require(MAGIC_END_TERM, file);
	
	return b;
}

struct UnOpTerm* readUnOpTerm(FILE* file, bool debug){
	
	if(debug){ printf("readUnOpTerm(...)\n"); }
	
	magic_num_require(MAGIC_UNOPTERM, file);
	
	const int opt = deserialize_int(file);
	
	struct UnOpTerm* t = make(UnOpTerm);
	
	t->op = (opt == OPT_PRESENT)? readOp(file, debug): NULL;
	
	t->term = readTerm(file, debug);
	
	magic_num_require(MAGIC_END_UNOPTERM, file);
	
	return t;
}

struct Range* readRange(FILE* file, bool debug){
	
	if(debug){ printf("readRange(...)\n"); }
	
	magic_num_require(MAGIC_RANGE, file);
	
	struct Range* r = make(Range);
	
	r->start = readExpr(file, debug);
	r->end = readExpr(file, debug);
	
	magic_num_require(MAGIC_END_RANGE, file);
	
	return r;
}

//statementnodes
struct Stmt* readStmt(FILE* file, bool debug){
	
	if(debug){ printf("readStmt(...)\n"); }
	
	magic_num_require(MAGIC_STMT, file);
	
	struct Stmt* b = make(Stmt);
	
	b->kind = deserialize_int(file);

	switch(b->kind){
		case 99: 
			{
				b->isBreak    = deserialize_int(file) == OPT_PRESENT;
				b->isContinue = deserialize_int(file) == OPT_PRESENT;
			}
			break;
		case 0: b->ptr.m0 = readLoopStmt(file, debug);   break;
		case 1: b->ptr.m1 = readMethodCall(file, debug); break;
		case 2: b->ptr.m2 = readWhileStmt(file, debug);  break;
		case 3: b->ptr.m3 = readIfStmt(file, debug);     break;
		case 4: b->ptr.m4 = readRetStmt(file, debug);    break;
		case 5: b->ptr.m5 = readAssignStmt(file, debug); break;
		case 7: b->ptr.m7 = readForStmt(file, debug);  	 break;
		case 8: b->ptr.m8 = readSwitchStmt(file, debug); break;
		default:
			printf("Error in readStmt\n");
			free(b);
			fclose(file);
			exit(1);
			break;
	}
	
	magic_num_require(MAGIC_END_STMT, file);
	
	return b;
}
struct IfStmt* readIfStmt(FILE* file, bool debug){
	
	if(debug){ printf("readIfStmt(...)\n"); }
	
	magic_num_require(MAGIC_IFSTMT, file);
	
	struct IfStmt* v = make(IfStmt);
	
	v->elseBlock = NULL;

	v->condition = readExpr(file, debug);

	v->block = readStmtBlock(file, debug);

	const int hasElse = deserialize_int(file);
	
	if(hasElse == OPT_PRESENT){
		v->elseBlock = readStmtBlock(file, debug);
	}
	
	magic_num_require(MAGIC_END_IFSTMT, file);
	
	return v;
}
struct WhileStmt* readWhileStmt(FILE* file, bool debug){
	
	if(debug){ printf("readWhileStmt(...)\n"); }
	
	magic_num_require(MAGIC_WHILESTMT, file);
	
	struct WhileStmt* v = make(WhileStmt);

	v->condition = readExpr(file, debug);
	v->block = readStmtBlock(file, debug);
	
	magic_num_require(MAGIC_END_WHILESTMT, file);
	
	return v;
}
struct RetStmt* readRetStmt(FILE* file, bool debug){
	
	if(debug){ printf("readRetStmt(...)\n"); }
	
	magic_num_require(MAGIC_RETSTMT, file);
	
	struct RetStmt* v = make(RetStmt);

	v->returnValue = readExpr(file, debug);
	
	magic_num_require(MAGIC_END_RETSTMT, file);
	
	return v;
}
struct AssignStmt* readAssignStmt(FILE* file, bool debug){
	
	if(debug){ printf("readAssignStmt(...)\n"); }
	
	magic_num_require(MAGIC_ASSIGNSTMT, file);
	
	const int option = deserialize_int(file);

	if(option != OPT_EMPTY && option != OPT_PRESENT){
		
		printf("Error in readAssignStmt\n");
		fclose(file);
		exit(1);
	}
	
	struct AssignStmt* v = make(AssignStmt);

	v->optType = NULL;

	if(option == OPT_PRESENT){
		v->optType = readType(file, debug);
	}

	v->var = readVariable(file, debug);
	
	char* assign_op = deserialize_string(file);
	
	strncpy(v->assign_op, assign_op, ASSIGNOP_LENGTH);
	
	free(assign_op);
	
	v->expr = readExpr(file, debug);
	
	magic_num_require(MAGIC_END_ASSIGNSTMT, file);

	return v;
}
struct MethodCall* readMethodCall(FILE* file, bool debug){
	
	if(debug){ printf("readMethodCall(...)\n"); }
	
	magic_num_require(MAGIC_METHODCALL, file);
	
	struct MethodCall* v = make(MethodCall);

	char* tmp = deserialize_string(file);
	strcpy(v->methodName, tmp);
	free(tmp);
	
	v->count_args = deserialize_int(file);

	v->args = 
		malloc(sizeof(struct Expr*)*(v->count_args));
	for(int i=0;i < (v->count_args);i++){
		v->args[i] = readExpr(file, debug);
	}
	
	magic_num_require(MAGIC_END_METHODCALL, file);
	
	return v;
}
struct LoopStmt* readLoopStmt(FILE* file, bool debug){
	
	if(debug){ printf("readLoopStmt(...)\n"); }
	
	magic_num_require(MAGIC_LOOPSTMT, file);
	
	struct LoopStmt* v = make(LoopStmt);

	v->count = readExpr(file, debug);
	v->block = readStmtBlock(file, debug);
	
	magic_num_require(MAGIC_END_LOOPSTMT, file);
	
	return v;
}
struct ForStmt* readForStmt(FILE* file, bool debug){
	
	if(debug){ printf("readForStmt(...)\n"); }
	
	magic_num_require(MAGIC_FORSTMT, file);
	
	char* indexName = deserialize_string(file);
	
	struct ForStmt* res = make(ForStmt);
	
	strncpy(res->indexName, indexName, DEFAULT_STR_SIZE);
	free(indexName);
	
	res->range = readRange(file, debug);
	res->block = readStmtBlock(file, debug);
	
	magic_num_require(MAGIC_END_FORSTMT, file);
	
	return res;
}
struct SwitchStmt* readSwitchStmt(FILE* file, bool debug){
	
	if(debug){ printf("readSwitchStmt(...)\n"); }
	
	magic_num_require(MAGIC_SWITCHSTMT, file);
	
	struct SwitchStmt* res = make(SwitchStmt);
	
	res->var = readVariable(file, debug);
	
	res->count_cases = deserialize_int(file);

	res->cases = malloc(sizeof(struct CaseStmt*)* (res->count_cases));
	
	for(int i=0; i < res->count_cases; i++){
		
		res->cases[i] = readCaseStmt(file, debug);
	}
	
	magic_num_require(MAGIC_END_SWITCHSTMT, file);
	
	return res;
}
struct CaseStmt* readCaseStmt(FILE* file, bool debug){
	
	if(debug){ printf("readCaseStmt(...)\n"); }
	
	magic_num_require(MAGIC_CASESTMT, file);
	
	struct CaseStmt* res = make(CaseStmt);
	
	res->kind = deserialize_int(file);
	
	res->ptr.m1 = NULL;
	res->block = NULL;
	
	switch(res->kind){
		case 0: res->ptr.m1 = readBoolConst(file, debug); break;
		case 1: res->ptr.m2 = readCharConst(file, debug); break;
		case 2: res->ptr.m3 = readIntConst(file, debug); break;
		default:
			printf("Error in readCase\n");
			free(res);
			fclose(file);
			exit(1);
	}
	
	const int hasBlock = deserialize_int(file);
	
	if(hasBlock == OPT_PRESENT){
		
		res->block = readStmtBlock(file, debug);
	}
	
	magic_num_require(MAGIC_END_CASESTMT, file);
	
	return res;
}
// --- typenodes -------------------------
struct Type* readType(FILE* file, bool debug){
	
	if(debug){ printf("readType(...)\n"); }
	
	magic_num_require(MAGIC_TYPE, file);
	
	const int kind = deserialize_int(file);;
	
	struct Type* b = make(Type);
	
	b->m1 = NULL;
	b->m2 = NULL;
	b->m3 = NULL;

	switch(kind){
		case 1: b->m1 = readBasicTypeWrapped(file, debug); break;
		case 2: b->m2 = readTypeParam(file, debug); break;
		case 3: b->m3 = readArrayType(file, debug); break;
		default:
			printf("Error in readType\n");
			free(b);
			fclose(file);
			exit(1);
	}
	
	magic_num_require(MAGIC_END_TYPE, file);
	
	return b;
}
struct SubrType* readSubrType(FILE* file, bool debug){
	
	if(debug){ printf("readSubrType(...)\n"); }
	
	magic_num_require(MAGIC_SUBRTYPE, file);
	
	struct SubrType* v = make(SubrType);

	v->returnType = readType(file, debug);
	
	v->hasSideEffects = deserialize_int(file);
	v->count_argTypes = deserialize_int(file);
	
	v->argTypes = malloc(sizeof(struct Type*)*(v->count_argTypes));
	
	for(int i=0;i < (v->count_argTypes); i++){
		v->argTypes[i] = readType(file, debug);
	}
	
	magic_num_require(MAGIC_END_SUBRTYPE, file);
	
	return v;
}
struct SimpleType* readSimpleType(FILE* file, bool debug){
	
	if(debug){ printf("readSimpleType(...)\n"); }
	
	magic_num_require(MAGIC_SIMPLETYPE, file);
	
	struct SimpleType* v = make(SimpleType);
	
	v->primitiveType = NULL;
	v->structType    = NULL;
	
	const int kind = deserialize_int(file);
	
	switch(kind){
		
		case 0: 
			v->primitiveType = readPrimitiveType(file, debug);
			break;
			
		case 1: 
			v->structType = readStructType(file, debug);
			break;
			
		default:
			printf("[AST][Error]");
			exit(1);
	}
	
	magic_num_require(MAGIC_END_SIMPLETYPE, file);
	
	return v;
}
struct ArrayType* readArrayType(FILE* file, bool debug){
	
	if(debug){ printf("readArrayType(...)\n"); }
	
	magic_num_require(MAGIC_ARRAYTYPE, file);
	
	struct ArrayType* v = make(ArrayType);
	
	v->element_type = readType(file, debug);
	
	magic_num_require(MAGIC_END_ARRAYTYPE, file);
	
	return v;
}
struct TypeParam* readTypeParam(FILE* file, bool debug){
	
	if(debug){ printf("readTypeParam(...)\n"); }
	
	magic_num_require(MAGIC_TYPEPARAM, file);
	
	struct TypeParam* v = make(TypeParam);
		
	fread(v, sizeof(struct TypeParam), 1, file);
	
	magic_num_require(MAGIC_END_TYPEPARAM, file);
	
	return v;
}
struct BasicTypeWrapped* readBasicTypeWrapped(FILE* file, bool debug){
	
	if(debug){ printf("readBasicTypeWrapped(...)\n"); }
	
	magic_num_require(MAGIC_BASICTYPEWRAPPED, file);
	
	const int kind = deserialize_int(file);
	
	struct BasicTypeWrapped* v = make(BasicTypeWrapped);

	v->simpleType = NULL;
	v->subrType = NULL;
	
	switch(kind){

		case 1: v->simpleType = readSimpleType(file, debug); break;
		case 2: v->subrType   = readSubrType(file, debug);   break;

		default:
			printf("Error in readBasicTypeWrapped\n");
			free(v);
			fclose(file);
			exit(1);
	}
	
	magic_num_require(MAGIC_END_BASICTYPEWRAPPED, file);
	
	return v;
}

struct StructType* readStructType(FILE* file, bool debug){
	
	magic_num_require(MAGIC_STRUCTTYPE, file);
	
	struct StructType* res = make(StructType);
	
	char* tmp = deserialize_string(file);
	strcpy(res->typeName, tmp);
	free(tmp);
	
	res->typeParamCount = deserialize_int(file);
	
	if(res->typeParamCount > 0){
		res->typeParams = malloc(sizeof(uint8_t)*(res->typeParamCount));
	}
	
	for(int i = 0; i < res->typeParamCount; i++){
		
		res->typeParams[i] = deserialize_int(file);
	}

	magic_num_require(MAGIC_END_STRUCTTYPE, file);
	
	return res;
}

struct PrimitiveType* readPrimitiveType(FILE* file, bool debug){
	
	magic_num_require(MAGIC_PRIMITIVETYPE, file);
	
	struct PrimitiveType* res = make(PrimitiveType);
	
	res->isIntType   = deserialize_int(file);
	res->isFloatType = deserialize_int(file);
	res->isCharType  = deserialize_int(file);
	res->isBoolType  = deserialize_int(file);
	
	res->intType = deserialize_int(file);
	
	magic_num_require(MAGIC_END_PRIMITIVETYPE, file);
	
	return res;
}

