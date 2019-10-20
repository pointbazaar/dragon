package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import static org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator.unique;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.StatementDracoVMCodeGenerator.generateDracoVMCodeForStatement;

public class LoopStatementDracoVMCodeGenerator {


  public static void genVMCodeForLoopStatement(
          LoopStatementNode loop,
          MethodNode containerMethod,
          DracoVMCodeWriter sb,
          SubroutineSymbolTable subTable,
          LocalVarSymbolTable varTable,
          StructsSymbolTable structsTable
  ) throws Exception {

    long unique=unique();
    String startlabel = "loopstart"+unique;
    String endlabel = "loopend"+unique;

    //push the expression
    genDracoVMCodeForExpression(loop.count,sb,subTable,varTable,structsTable); //+1
    sb.dup(); //+1

    sb.label(startlabel);

    //if counter is 0, jump to end
    sb.iconst(0); //+1
    sb.eq(); //-1
    sb.if_goto(endlabel); //-1

    //execute statements
    for(StatementNode stmt : loop.statements){
      generateDracoVMCodeForStatement(stmt,containerMethod,sb,subTable,varTable,structsTable);
    }

    //subtract 1 from the counter
    sb.dec();

    //duplicate top of stack so we can compare again
    sb.dup(); //+1
    sb._goto(startlabel);

    sb.label(endlabel);
    sb.pop(); //-1
  }
}
