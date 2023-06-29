package me.iipho3nix.simpleasm;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Interpreter {
    private Map<String, List<List<String>>> program;
    private String args;
    public Map<Long, Long> memory = new HashMap<>();

    public Interpreter(Map<String, List<List<String>>> tokenizedProgram, String args) {
        program = tokenizedProgram;
        this.args = args;
    }

    public void interpret() {
        List<List<String>> mainBlock = program.get("<main>");
        executeBlock(mainBlock);
    }

    public void executeBlock(List<List<String>> instructionsList) {
        boolean stop = false;
        for (List<String> instructions : instructionsList) {
            String instruction = instructions.get(0);
            List<String> arguments = instructions.subList(1, instructions.size());

            if (stop) {
                break;
            }

            switch (instruction) {
                case "ADD":
                    memory.put(Long.parseLong(arguments.get(2)), memory.get(Long.parseLong(arguments.get(0))) + memory.get(Long.parseLong(arguments.get(1))));
                    break;
                case "SUB":
                    memory.put(Long.parseLong(arguments.get(2)), memory.get(Long.parseLong(arguments.get(0))) - memory.get(Long.parseLong(arguments.get(1))));
                    break;
                case "MUL":
                    memory.put(Long.parseLong(arguments.get(2)), memory.get(Long.parseLong(arguments.get(0))) * memory.get(Long.parseLong(arguments.get(1))));
                    break;
                case "DIV":
                    memory.put(Long.parseLong(arguments.get(2)), memory.get(Long.parseLong(arguments.get(0))) / memory.get(Long.parseLong(arguments.get(1))));
                    break;
                case "REM":
                    memory.put(Long.parseLong(arguments.get(2)), memory.get(Long.parseLong(arguments.get(0))) % memory.get(Long.parseLong(arguments.get(1))));
                    break;
                case "STR":
                    memory.put(Long.parseLong(arguments.get(1)), Long.parseLong(arguments.get(0)));
                    break;
                case "CPY":
                    memory.put(Long.parseLong(arguments.get(1)), memory.get(Long.parseLong(arguments.get(0))));
                    break;
                case "AND":
                    memory.put(Long.parseLong(arguments.get(2)), memory.get(Long.parseLong(arguments.get(0))) & memory.get(Long.parseLong(arguments.get(1))));
                    break;
                case "OR":
                    memory.put(Long.parseLong(arguments.get(2)), memory.get(Long.parseLong(arguments.get(0))) | memory.get(Long.parseLong(arguments.get(1))));
                    break;
                case "XOR":
                    memory.put(Long.parseLong(arguments.get(2)), memory.get(Long.parseLong(arguments.get(0))) ^ memory.get(Long.parseLong(arguments.get(1))));
                    break;
                case "NOT":
                    memory.put(Long.parseLong(arguments.get(1)), ~memory.get(Long.parseLong(arguments.get(0))));
                    break;
                case "JMP":
                    executeBlock(program.get(arguments.get(0)));
                    stop = true;
                    break;
                case "JMPBZ":
                    if (memory.get(Long.parseLong(arguments.get(0))) > 0) {
                        executeBlock(program.get(arguments.get(1)));
                        stop = true;
                    }
                    break;
                case "JMPZ":
                    if (memory.get(Long.parseLong(arguments.get(0))) == 0L) {
                        executeBlock(program.get(arguments.get(1)));
                        stop = true;
                    }
                    break;
                case "JMPIF":
                    if (memory.get(Long.parseLong(arguments.get(0))).equals(memory.get(Long.parseLong(arguments.get(1))))) {
                        executeBlock(program.get(arguments.get(2)));
                        stop = true;
                    }
                    break;
                case "SOUT":
                    long start = Long.parseLong(arguments.get(0));
                    long length = memory.get(Long.parseLong(arguments.get(1)));
                    StringBuilder sb = new StringBuilder();

                    for (long i = start; i < start + length; i++) {
                        char asciiChar = (char) memory.get(i).longValue();
                        sb.append(asciiChar);
                    }

                    String str = sb.toString();
                    System.out.println(str);
                    break;
                case "SIN":
                    System.err.println("SIN Instruction is unsupported");
                    stop = true;
                    break;
                case "RIN":
                    int i2 = 0;
                    char[] charArray = args.toCharArray();
                    int[] asciiArray = new int[charArray.length];

                    for (int i = 0; i < charArray.length; i++) {
                        asciiArray[i] = (int) charArray[i];
                    }

                    for (int asciiValue : asciiArray) {
                        memory.put(Long.parseLong(arguments.get(0)) + i2, (long) asciiValue);
                        i2++;
                    }

                    memory.put(Long.parseLong(arguments.get(1)), (long) i2);
                    break;
                case "RF":
                    System.err.println("RF Instruction is unsupported");
                    stop = true;
                    break;
                case "WF":
                    System.err.println("WF Instruction is unsupported");
                    stop = true;
                    break;
                case "HALT":
                    System.out.println("Program exited with code " + arguments.get(0));
                    stop = true;
                    break;
                default:
                    System.err.println("Unknown instruction: " + instruction);
                    stop = true;
                    break;
            }
        }
    }

}
