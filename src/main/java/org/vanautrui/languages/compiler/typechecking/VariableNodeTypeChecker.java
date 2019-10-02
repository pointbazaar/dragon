package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.ClassFieldNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.VariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.List;

public class VariableNodeTypeChecker {


  static void typeCheckVariableNode(List<AST> asts, ClassNode classNode, MethodNode methodNode, VariableNode variableNode, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception{
    //it should check that the variable is
    //declared in method scope or class scope.
    //so there should be some declaration of it

    if(variableNode.indexOptional.isPresent()){
      //if there is an index, it should be positive. we can check one of the bounds for free
      //by only accepting PInt type
      ITypeNode index_type = TypeResolver.getTypeExpressionNode(variableNode.indexOptional.get(), methodNode, subTable, varTable);
      if(!index_type.getTypeName().equals("PInt")){
        throw new Exception("can only index into arrays with PInt type. Because an array index is >= 0.");
      }
    }


    //identifiers can only be used within a class or method so
    //there should be a context

    for(ClassFieldNode fieldNode : classNode.fieldNodeList){
      if(fieldNode.name.equals(variableNode.name)){
        //found the identifier declared here
        return;
      }
    }

    //search if identifier is declared as a local variable or argument variable
    if(varTable.containsVariable(variableNode.name)){
      return;
    }

    //search if identifier is declared as a subroutine
    if(subTable.containsSubroutine(variableNode.name)){
      if(!variableNode.indexOptional.isPresent()){
        return;
      }else{
        throw new Exception("TypeChecker: '"+variableNode.name+"' has been used with an index, : '"+variableNode.toSourceCode()+"' , but it is a subroutine. you cannot index into subroutines");
      }
    }

    throw new Exception("could not find declaration for usage of variable '"+variableNode.name+"' \n"+subTable.toString());
  }
}
