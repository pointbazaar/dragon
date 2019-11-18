package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SubroutineTypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.VariableDracoVMCodeGenerator.*;

public final class MethodCallDracoVMCodeGenerator {

    public static List<String> genVMCodeForMethodCall(
            MethodCallNode methodCallNode,
            SubroutineSymbolTable subTable,
            LocalVarSymbolTable varTable,
            StructsSymbolTable structsTable
    ) throws Exception {

        final List<String> instrs = new ArrayList<>();

        //push arguments on stack in reverse order
        for (int i = methodCallNode.argumentList.size() - 1; i >= 0; i--) {
            final ExpressionNode arg = methodCallNode.argumentList.get(i);
            instrs.addAll(genDracoVMCodeForExpression(arg, subTable, varTable, structsTable));

        }
        final int nArgs;

        if (varTable.containsVariable(methodCallNode.methodName)) {
            //compile call to subroutine which is given as local variable or argument

            //push the label on the stack from either LOCAL SEGMENT or ARG SEGMENT
            instrs.addAll(genDracoVMCodeForSimpleVariable(methodCallNode.methodName, Optional.empty(), subTable, varTable, structsTable));

            //perform a call to the label on stack
            instrs.add("callfromstack");

            //very hacky. very dirty. should be cleaner. like with polymorphy.
            //different types implementing an interface which is then in a collection in the local var symbol table
            final ITypeNode type = varTable.get(methodCallNode.methodName).getType().type;
            final IBasicAndWrappedTypeNode type1 = ((BasicTypeWrappedNode) type).typenode;
            final SubroutineTypeNode type2 = (SubroutineTypeNode) type1;
            final int mynargs = type2.argumentTypes.size();
            nArgs = mynargs;
            //nArgs = ((SubroutineTypeNode)varTable.get(methodCallNode.methodName).getType().type).argumentTypes.size();
        } else if (subTable.containsSubroutine(methodCallNode.methodName)) {

            instrs.add("call "+subTable.getContainingClassName(methodCallNode.methodName)+"_"+methodCallNode.methodName);

            nArgs = subTable.getNumberOfArgumentsOfSubroutine(methodCallNode.methodName);
        } else {
            throw new Exception("subroutine " + methodCallNode.methodName + " not found in local variables and also not found in subroutines");
        }
        //caller removes the arguments off the stack
        for (int i = 0; i < nArgs; i++) {

            //we must swap first, as there is the return value on top of stack
            //remove previously pushed arguments off the stack
            //remove previously pushed arguments off the stack
            instrs.add("swap");
            instrs.add("pop");
        }
        return instrs;
    }
}
