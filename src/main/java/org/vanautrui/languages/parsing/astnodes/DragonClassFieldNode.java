package org.vanautrui.languages.parsing.astnodes;

import org.vanautrui.languages.lexing.tokens.AccessModifierToken;
import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.parsing.IDragonASTNode;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public class DragonClassFieldNode implements IDragonASTNode {

    public DragonAccessModifierNode access;

    public DragonTypeNode type;

    public DragonIdentifierNode name;

    public DragonClassFieldNode(List<DragonToken> tokens)throws Exception{

        List<DragonToken> copy = new ArrayList<>(tokens);

        try{
            this.access=new DragonAccessModifierNode(copy);
        }catch (Exception e){
            //pass
        }

        this.type=new DragonTypeNode(copy);

        this.name=new DragonIdentifierNode(copy);

        tokens.clear();
        tokens.addAll(copy);
    }

    @Override
    public String toSourceCode() {
        return null;
    }
}
