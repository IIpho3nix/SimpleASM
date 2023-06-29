package me.iipho3nix.simpleasm;

import java.util.List;
import java.util.Map;

public class Main {
    public static void main(String[] args) {
        //Example Usage
        String code =
                "<main>:\n" +
                        "    STR 74,11 -- store value 74 at address 11\n" +
                        "    JMP <iseven>\n" +
                        "\n" +
                        "<print>:\n" +
                        "    SOUT 15,12\n" +
                        "    HALT 0\n" +
                        "\n" +
                        "<iseven>:\n" +
                        "    STR 1,12 -- store 1 at address 12\n" +
                        "    AND 11,12,13 -- AND address 11 (external input) with 1 and save result to address 13\n" +
                        "    JMPZ 13,<even> -- jump to even if even\n" +
                        "    JMP <odd> -- jump to odd\n" +
                        "\n" +
                        "<even>:\n" +
                        "    STR 69,15 -- store ascii \"E\" in address 15 (external output)\n" +
                        "    JMP <print>\n" +
                        "\n" +
                        "<odd>:\n" +
                        "    STR 79,15 -- store ascii \"O\" in address 15 (external output)\n" +
                        "    JMP <print>";

        Map<String, List<List<String>>> tokenized = Tokenizer.tokenize(code);

        Interpreter interpreter = new Interpreter(tokenized, String.join(" ", args));

        interpreter.interpret();
    }
}