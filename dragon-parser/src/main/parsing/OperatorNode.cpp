
//standard headers
#include <vector>
#include <set>
#include <string>
#include <map>

//project headers
#include "OperatorNode.hpp"
#include "../../../lexing/TokenList.hpp"

using namespace std;


OperatorNode::OperatorNode(TokenList tokens) {

	TokenList copy = new TokenList(tokens);

	IToken token = copy.get(0);

	if (token instanceof OperatorToken) {

		if (copy.get(1) instanceof OperatorToken) {
			final OperatorToken opleft = (OperatorToken) token;
			final OperatorToken opright = (OperatorToken) copy.get(1);

			if ((opleft.operator.equals("<")) || opleft.operator.equals(">")
					&& opright.operator.equals("=")
			) {
				this.operator = opleft.operator + opright.operator;
				copy.consume(2);
			} else if (opleft.operator.equals("=") && opright.operator.equals("=")) {
				this.operator = "==";
				copy.consume(2);
			} else {
				throw "could not recognize operator token: got " + token.getContents();
			}
		} else {
			this.operator = ((OperatorToken) token).operator;
			copy.consume(1);
		}
	} else {
		throw "could not recognize operator token: got " + token.getContents();
	}

	tokens.set(copy);
}

