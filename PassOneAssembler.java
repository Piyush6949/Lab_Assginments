import java.io.*;
import java.util.*;

class PassOneAssembler {
    // Data Structures
    static Map<String, Integer> optab = new HashMap<>();
    static Map<String, Symbol> symtab = new LinkedHashMap<>();
    static List<Literal> littab = new ArrayList<>();
    static List<Integer> pooltab = new ArrayList<>();

    static int LC = 0;
    static int poolIndex = 0;

    public static void main(String[] args) throws IOException {
        loadOptab(); // Load operation codes
        pooltab.add(0); // Initialize first pool

        BufferedReader br = new BufferedReader(new FileReader("input.asm"));
        String line;

        while ((line = br.readLine()) != null) {
            processLine(line);
        }

        assignLiterals();

        printTables();
    }

    static void loadOptab() {
        // Example opcodes - customize for your problem
        optab.put("MOVER", 1);
        optab.put("MOVEM", 2);
        optab.put("ADD", 3);
        optab.put("SUB", 4);
        optab.put("START", 0);
        optab.put("END", 0);
        optab.put("LTORG", 0);
    }

    static void processLine(String line) {
        String[] tokens = line.trim().split("\\s+");
        int index = 0;

        // If label present
        if (!tokens[0].isEmpty() && !optab.containsKey(tokens[0])) {
            // Insert label into SYMTAB with current LC
            symtab.putIfAbsent(tokens[0], new Symbol(tokens[0], LC));
            index++;
        }

        String opcode = tokens[index];
        index++;

        if (opcode.equals("START")) {
            LC = Integer.parseInt(tokens[index]);
        } else if (opcode.equals("END") || opcode.equals("LTORG")) {
            assignLiterals();
        } else if (optab.containsKey(opcode)) {
            System.out.println(LC + "\t" + line);
            LC += 1; // For simplicity, assume one word per instruction
            // Check for literals in operands and add to LITTAB
            for (int i = index; i < tokens.length; i++) {
                if (tokens[i].startsWith("=")) {
                    littab.add(new Literal(tokens[i], -1));
                }
            }
        } else {
            // Handle directives like ORIGIN, EQU etc. if needed
        }
    }

    static void assignLiterals() {
        for (int i = pooltab.get(poolIndex); i < littab.size(); i++) {
            Literal lit = littab.get(i);
            if (lit.address == -1) {
                lit.address = LC;
                System.out.println(LC + "\t" + lit.literal);
                LC++;
            }
        }
        poolIndex++;
        pooltab.add(littab.size());
    }

    static void printTables() {
        System.out.println("\nSYMTAB:");
        for (Symbol s : symtab.values()) {
            System.out.println(s.name + "\t" + s.address);
        }

        System.out.println("\nLITTAB:");
        for (Literal l : littab) {
            System.out.println(l.literal + "\t" + l.address);
        }

        System.out.println("\nPOOLTAB:");
        for (int i : pooltab) {
            System.out.println(i);
        }
    }

    static class Symbol {
        String name;
        int address;

        Symbol(String name, int address) {
            this.name = name;
            this.address = address;
        }
    }

    static class Literal {
        String literal;
        int address;

        Literal(String literal, int address) {
            this.literal = literal;
            this.address = address;
        }
    }
}
