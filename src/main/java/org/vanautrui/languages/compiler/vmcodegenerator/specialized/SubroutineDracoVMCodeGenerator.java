package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.StatementDracoVMCodeGenerator.generateDracoVMCodeForStatement;

public class SubroutineDracoVMCodeGenerator {


  public static void generateDracoVMCodeForMethod(MethodNode m, DracoVMCodeWriter sb, SubroutineSymbolTable subTable)throws Exception{

    LocalVarSymbolTable varTable = SymbolTableGenerator.createMethodScopeSymbolTable(m,subTable);

    sb.subroutine(m.methodName,m.arguments.size(),subTable.getNumberOfLocalVariablesOfSubroutine(m.methodName));
    //not sure if it is number of arguments or number of local vars

    //push the number of local variables on the stack
    for(int i=0;i<subTable.getNumberOfLocalVariablesOfSubroutine(m.methodName);i++){
      sb.iconst(0,"push local vars on the stack");
    }

    //push ebp
    //mov esp ebp

    for(StatementNode stmt : m.statements){
      generateDracoVMCodeForStatement(stmt,m,sb,subTable,varTable);
    }

    //return should be the last statement in every possible branch for these statements


  }
}