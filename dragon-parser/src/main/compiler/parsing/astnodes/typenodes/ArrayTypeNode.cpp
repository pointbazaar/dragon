
//standard headers
#include <vector>
#include <set>

//project headers
#include "ArrayTypeNode.hpp"

class ArrayTypeNode : IASTNode, ITypeNode {

public:
	TypeNode element_type;

	ArrayTypeNode(TypeNode element_type) {
		this.element_type = element_type;
	}

	ArrayTypeNode(TokenList tokens) throws Exception {

		TokenList copy1 = tokens.copy();

		copy1.expectAndConsumeOtherWiseThrowException(new LBracketToken());
		this.element_type = new TypeNode(copy1);
		copy1.expectAndConsumeOtherWiseThrowException(new RBracketToken());


		tokens.set(copy1);
	}

};