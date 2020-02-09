
#include <vector>
#include <iostream>

#include "WhileStmt.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/Token.hpp"
#include "../Expr.hpp"
#include "Stmt.hpp"

using namespace std;

WhileStmt::WhileStmt(TokenList tokens, bool debug){

	if(debug){
		cout << "WhileStmt(...)" << endl;
	}

	TokenList copy = TokenList(tokens);

	copy.expect(Token(WHILE));

	this->condition = new Expr(copy,debug);

	copy.expect(Token(LCURLY));

	Token next = copy.get(0);
	while (!(next.kind == RCURLY)) {
		this->statements.push_back(new Stmt(copy,debug));
		next = copy.get(0);
	}

	copy.expect(Token(RCURLY));

	tokens.set(copy);
}
