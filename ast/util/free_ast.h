#ifndef FREEAST
#define FREEAST

#include "../ast.h"

//this header file declares all subroutines
//for freeing the memory allocated for AST Nodes.

//these subroutines are separated from the
//constructor subroutines in the parser,
//as other modules (transpiler, etc.)
//must also be able to free AST Nodes,
//as they could obtain such via ast_reader.c

void free_ast(struct AST* ast);

//const
void free_bool_const(struct BoolConst* bc);
void free_char_const(struct CharConst* cc);
void free_float_const(struct FloatConst* fc);
void free_int_const(struct IntConst* ic);
void free_hex_const(struct HexConst* hc);
void free_bin_const(struct BinConst* hc);
void free_const_value(struct ConstValue* cv);
void free_string_const(struct StringConst* s);

//subr
void free_decl_arg(struct DeclArg* da);
void free_method(struct Method* m);
void free_method_decl(struct MethodDecl* m);
void free_externc(struct ExternC* ec);

//other
void free_identifier(struct Identifier* id);
void free_namespace(struct Namespace* ns);
void free_stmt_block(struct StmtBlock* block);
void free_range(struct Range* range);
void free_lambda(struct Lambda* l);

//struct
void free_struct_decl(struct StructDecl* sd);
void free_struct_member(struct StructMember* sm);

//expr
void free_expr(struct Expr* expr);
void free_term(struct Term* t);
void free_un_op_term(struct UnOpTerm* t);
void free_op(struct Op* op);

//var
void free_variable(struct Variable* var);
void free_simple_var(struct SimpleVar* sv);

//stmts
void free_assign_stmt(struct AssignStmt* as);
void free_if_stmt(struct IfStmt* is);
void free_loop_stmt(struct LoopStmt* is);
void free_call(struct Call* mc);
void free_ret_stmt(struct RetStmt* rs);
void free_stmt(struct Stmt* s);
void free_while_stmt(struct WhileStmt* ws);
void free_for_stmt(struct ForStmt* fstmt);
void free_switch_stmt(struct SwitchStmt* sstmt);
void free_case_stmt(struct CaseStmt* cstmt);
void free_try_catch_stmt(struct TryCatchStmt* tcs);

//types
void free_array_type(struct ArrayType* at);
void free_basic_type(struct BasicType* btw);
void free_simple_type(struct SimpleType* st);
void free_subr_type(struct SubrType* st);
void free_type(struct Type* t);
void free_type_param(struct TypeParam* tp);
void free_primitive_type(struct PrimitiveType* p);
void free_struct_type(struct StructType* s);

#endif
