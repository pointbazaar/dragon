package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.CharacterList;

public class SymbolToken implements DragonToken {

    public static final String[] symbols = new String[]{
            "{", "}",
            "[", "]",
            "(", ")",
            ","
    };

    public String symbol;

    public SymbolToken(CharacterList list) throws Exception {

        //should be for keywords such as {,},[,],+,-,.,','

        for (String sym : symbols) {
            if (list.startsWith(sym)) {
                this.symbol = sym;
                list.consumeTokens(sym.length());
                return;
            }
        }

        throw new Exception("could not recognize a keyword");
    }

    public SymbolToken(String newcontents) throws Exception {
        this(new CharacterList(newcontents));
    }

    @Override
    public String getContents() {
        return this.symbol;
    }

    @Override
    public boolean equals(Object other) {
        if (other == null) {
            return false;
        }

        if (other instanceof SymbolToken) {

            return this.symbol.equals(
                    ((SymbolToken) other).symbol
            );
        }

        return false;
    }
}