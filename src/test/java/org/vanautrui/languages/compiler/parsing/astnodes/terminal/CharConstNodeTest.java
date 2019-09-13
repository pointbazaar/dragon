package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.lexing.tokens.CharConstantToken;

public class CharConstNodeTest {

    @Test
    public void test_parse_char_constant_node() throws Exception {
        TokenList list = new TokenList();
        list.add(new CharConstantToken(new CharacterList("'h'")));

        CharConstNode node = new CharConstNode(list);
        Assert.assertEquals('h',node.content);
    }
}