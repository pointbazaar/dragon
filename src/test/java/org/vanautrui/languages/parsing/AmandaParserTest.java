package org.vanautrui.languages.parsing;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.model.AmandaAST;
import org.vanautrui.languages.model.tokens.AmandaToken;
import org.vanautrui.languages.model.tokens.ClassToken;
import org.vanautrui.languages.model.tokens.IdentifierToken;

import java.util.ArrayList;

public class AmandaParserTest {

    AmandaParser parser=new AmandaParser();

    @Test
    public void test_can_create_correct_classnode() throws Exception{
        ArrayList<AmandaToken> tokens=new ArrayList<>();
        tokens.add(new ClassToken("class"));

        String classname = "exampleclass";

        tokens.add(new IdentifierToken(classname));
        tokens.add(new AmandaToken("\n"));

        AmandaAST ast = parser.parse(tokens);

        Assert.assertEquals(classname, ast.classNodeList.get(0).getName());
    }
}
