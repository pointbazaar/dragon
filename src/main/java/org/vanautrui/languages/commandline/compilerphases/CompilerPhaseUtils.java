package org.vanautrui.languages.commandline.compilerphases;

import org.fusesource.jansi.Ansi;
import org.simpleframework.xml.strategy.Strategy;
import org.simpleframework.xml.strategy.Type;
import org.simpleframework.xml.strategy.Visitor;
import org.simpleframework.xml.strategy.VisitorStrategy;
import org.simpleframework.xml.stream.InputNode;
import org.simpleframework.xml.stream.NodeMap;
import org.simpleframework.xml.stream.OutputNode;
import org.vanautrui.languages.TerminalUtil;

import java.awt.*;

import static org.fusesource.jansi.Ansi.Color.GREEN;
import static org.fusesource.jansi.Ansi.Color.RED;
import static org.fusesource.jansi.Ansi.ansi;

public class CompilerPhaseUtils {
    public static void printBeginPhase(String phaseName,boolean printLongForm) throws InterruptedException {
        //TerminalUtil.print(String.format("%-18s ",phaseName),Ansi.Color.GREEN);
        Ansi ansi = Ansi.ansi();
        if(!printLongForm) {
            ansi.eraseLine(Ansi.Erase.BACKWARD);
        }

        ansi.fg(GREEN);
        ansi.a(phaseName);

        ansi.reset();
        System.out.print(ansi);
        System.out.flush();

        //rip this out, this is slowing down the compiler
        //Thread.sleep(100);
    }

    public static void printEndPhase(boolean success, boolean printLongForm) throws Exception{
        Ansi ansi1=Ansi.ansi();
        if(!printLongForm) {
            ansi1.cursorToColumn(0);
        }

        ansi1.cursorToColumn(18);
        if(success) {
            ansi1.fg(GREEN);
            ansi1.a("✓");
        }else{
            ansi1.fg(RED);
            ansi1.a("⚠");
        }
        ansi1.cursorToColumn(0);
        if(printLongForm){
            ansi1.newline();
        }
        if(!printLongForm) {
            ansi1.eraseLine(Ansi.Erase.FORWARD);
        }
        ansi1.reset();
        System.out.print(ansi1);
        System.out.flush();

        //rip this out, this is slowing down the compiler
        //Thread.sleep(100);
    }
    public static void printBuildConclusion(boolean success){
        if(success) {
            System.out.println(ansi().fg(GREEN).bold().a("BUILD SUCCESS").reset());
        }else {
            System.out.println(ansi().fg(RED).bold().a("BUILD FAILURE").reset());
        }
    }

    public static void printDuration(long start,long end){
        long duration=end-start;
        System.out.println(String.format("Duration: %6sms",duration));
    }

    public static final Strategy getPreferredXMLSerializationStrategyHumanReadable(){
        Strategy strategy = new VisitorStrategy(new Visitor() {
            @Override
            public void read(Type type, NodeMap<InputNode> nodeMap) throws Exception {

            }

            @Override
            public void write(Type type, NodeMap<OutputNode> nodeMap) throws Exception {
                nodeMap.remove("class");
            }
        });
        return strategy;
    };

    public static void printDurationFeedback(long duration /*milliseconds*/){
        String str = duration + " ms";
        if(duration>500) {
            TerminalUtil.println("☠ "+str+" Compilation took too long. This needs to be fixed. Please file an Issue on GitHub.", Ansi.Color.RED);
        }else if(duration>200) {
            TerminalUtil.println("☠ "+str+" we are truly sorry for the delay :(", Ansi.Color.YELLOW);
        }else if(duration>100){
            TerminalUtil.println("✝ "+str+" sorry it took so long!", Ansi.Color.YELLOW);
        }else {
            TerminalUtil.println("☕ " + str, Ansi.Color.GREEN);
        }
    }
}
