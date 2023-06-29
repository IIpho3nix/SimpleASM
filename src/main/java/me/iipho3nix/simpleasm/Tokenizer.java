package me.iipho3nix.simpleasm;

import java.util.*;

public class Tokenizer {
    public static Map<String, List<List<String>>> tokenize(String code) {
        Map<String, List<List<String>>> tokens = new HashMap<>();

        String[] lines = code.split("\n");
        String currentLabel = null;

        for (String line : lines) {
            String noComments = line.replaceAll("\\s*--.*", "");
            String trimmed = noComments.trim();

            if (trimmed.isBlank())
                continue;

            if(trimmed.startsWith("<") && trimmed.endsWith(">:")) {
                currentLabel = trimmed.substring(0, trimmed.length() - 1);
                tokens.put(currentLabel, new ArrayList<>());
            }

            if (currentLabel != null && !(trimmed.startsWith("<") && trimmed.endsWith(">:"))) {
                List<String> tokenized = splitCode(trimmed);
                tokens.get(currentLabel).add(tokenized);
            }
        }

        return tokens;
    }

    private static List<String> splitCode(String code) {
        String pattern = "\\s+|,\\s*";
        return Arrays.asList(code.split(pattern));
    }
}

