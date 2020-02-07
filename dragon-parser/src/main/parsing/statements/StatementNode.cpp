
//standard headers
#include <vector>
#include <iostream>
//project headers
#include "StatementNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/BaseToken.hpp"
#include "WhileStatementNode.hpp"
#include "IfStatementNode.hpp"
#include "ReturnStatementNode.hpp"
#include "MethodCallNode.hpp"
#include "AssignmentStatementNode.hpp"

StatementNode::StatementNode(TokenList tokens, bool debug) {

	if(debug){
		cout << "StatementNode(...)" << endl;
	}

	TokenList copy = tokens.copy();

	if (copy.size() == 0) {
		throw "tried to parse a Statement, but there are no tokens left";
	}

	BaseToken first = copy.get(0);

	if (first.kind == LOOP) {
		//this->statementNode = LoopStatementNode(copy);
	} else if (first.kind == WHILE) {
		this->m2 = new WhileStatementNode(copy,debug);
	} else if (first.kind == IF) {
		this->m3 = new IfStatementNode(copy,debug);
	} else if (first.kind == RETURN) {
		this->m4 = new ReturnStatementNode(copy,debug);
	} else {
		//TODO: we have to figure something out here.
		//i don't want 'let' statements
		//because it just messes up the code and is
		//just bloat to write.
		//but parsing should be straightforward. to give good error messages

		try {
			TokenList copy2 = copy.copy();
			this->m1 = new MethodCallNode(copy2,debug);
			copy2.expectAndConsumeOtherWiseThrowException(BaseToken(SEMICOLON));

			copy.set(copy2);
		} catch (string e1) {
			this->m5 = new AssignmentStatementNode(copy,debug);
		}
	}

	tokens.set(copy);
}

