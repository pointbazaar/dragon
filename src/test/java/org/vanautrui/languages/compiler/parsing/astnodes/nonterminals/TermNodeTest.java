package org.vanautrui.languages.compiler.parsing.astnodes.nonterminals;

import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;

public class TermNodeTest {

    @Test
    public void test_simple_term() throws Exception {

        TokenList list = new TokenList();
        list.add(new IntegerNonNegativeConstantToken(4));
        TermNode expr = new TermNode(list);
    }

    @Test
    public void test_variable_term() throws Exception {

        TokenList list = new TokenList();
        list.add(new IdentifierToken("x"));
        TermNode expr = new TermNode(list);
    }


}