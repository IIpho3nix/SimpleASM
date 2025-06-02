#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>

std::map<std::string, std::vector<std::vector<std::string>>> tokenize(const std::string& code) {
    std::map<std::string, std::vector<std::vector<std::string>>> tokens;
    std::string currentLabel;
    std::regex labelRegex(R"(<(\w+)>:)");

    std::istringstream iss(code);
    for (std::string line; std::getline(iss, line); ) {
        std::string noComments = std::regex_replace(line, std::regex("\\s*--.*"), "");
        std::string trimmed = std::regex_replace(noComments, std::regex("^\\s+|\\s+$"), "");
        if (trimmed.empty()) continue;

        std::smatch match;
        if (std::regex_match(trimmed, match, labelRegex)) {
            currentLabel = "<" + match[1].str() + ">";
            tokens[currentLabel] = {};
        } else {
            std::istringstream lineIss(trimmed);
            std::vector<std::string> tokenized;
            for (std::string token; std::getline(lineIss, token, ' '); ) {
                std::istringstream tokenIss(token);
                std::string instruction;
                std::getline(tokenIss, instruction, ',');
                tokenized.push_back(instruction);
                std::string arguments;
                std::getline(tokenIss, arguments);
                std::istringstream argumentsIss(arguments);
                for (std::string arg; std::getline(argumentsIss, arg, ','); ) {
                    tokenized.push_back(arg);
                }
            }
            tokens[currentLabel].push_back(tokenized);
        }
    }
    return tokens;
}

std::string handleFunc(const std::string &funcName) {
    if (funcName == "main") {
        return "progMain";
    } else {
        return funcName;
    }
}

void generateCpp(const std::map<std::string, std::vector<std::vector<std::string>>>& tokens, const std::string& outputPath) {
    std::ofstream out(outputPath);
    out << "#include <iostream>\n#include <map>\n#include <vector>\n#include <string>\n#include <cstdlib>\n\n";
    out << "std::map<long long, long long> memory;\nstd::vector<std::string> args;\n\n";
    out << "using namespace std;\n\n";

    for (const auto& [label, instructions] : tokens) {
        out << "void " << handleFunc(label.substr(1, label.length() - 2)) << "();" << "\n";
    }

    out << "\n";

    for (const auto& [label, instructions] : tokens) {
        out << "void " << handleFunc(label.substr(1, label.length() - 2)) << "() {" << "\n";
        for (const auto& instr : instructions) {
            const std::string& op = instr[0];
            if (op == "ADD") out << "    memory[stoll(\"" << instr[3] << "\")] = memory[stoll(\"" << instr[1] << "\")] + memory[stoll(\"" << instr[2] << "\")];\n";
            else if (op == "SUB") out << "    memory[stoll(\"" << instr[3] << "\")] = memory[stoll(\"" << instr[1] << "\")] - memory[stoll(\"" << instr[2] << "\")];\n";
            else if (op == "MUL") out << "    memory[stoll(\"" << instr[3] << "\")] = memory[stoll(\"" << instr[1] << "\")] * memory[stoll(\"" << instr[2] << "\")];\n";
            else if (op == "DIV") out << "    memory[stoll(\"" << instr[3] << "\")] = memory[stoll(\"" << instr[1] << "\")] / memory[stoll(\"" << instr[2] << "\")];\n";
            else if (op == "REM") out << "    memory[stoll(\"" << instr[3] << "\")] = memory[stoll(\"" << instr[1] << "\")] % memory[stoll(\"" << instr[2] << "\")];\n";
            else if (op == "STR") out << "    memory[stoll(\"" << instr[2] << "\")] = " << instr[1] << ";\n";
            else if (op == "CPY") out << "    memory[stoll(\"" << instr[2] << "\")] = memory[stoll(\"" << instr[1] << "\")];\n";
            else if (op == "AND") out << "    memory[stoll(\"" << instr[3] << "\")] = memory[stoll(\"" << instr[1] << "\")] & memory[stoll(\"" << instr[2] << "\")];\n";
            else if (op == "OR") out << "    memory[stoll(\"" << instr[3] << "\")] = memory[stoll(\"" << instr[1] << "\")] | memory[stoll(\"" << instr[2] << "\")];\n";
            else if (op == "XOR") out << "    memory[stoll(\"" << instr[3] << "\")] = memory[stoll(\"" << instr[1] << "\")] ^ memory[stoll(\"" << instr[2] << "\")];\n";
            else if (op == "NOT") out << "    memory[stoll(\"" << instr[2] << "\")] = ~memory[stoll(\"" << instr[1] << "\")];\n";
            else if (op == "JMP") out << "     {" << handleFunc(instr[1].substr(1, instr[1].length() - 2))  << "(); return;}\n";
            else if (op == "JMPBZ") out << "    if (memory[stoll(\"" << instr[1] << "\")] > 0) {" << handleFunc(instr[2].substr(1, instr[2].length() - 2))  << "(); return;}\n";
            else if (op == "JMPZ") out << "    if (memory[stoll(\"" << instr[1] << "\")] == 0) {" << handleFunc(instr[2].substr(1, instr[2].length() - 2))  << "(); return;}\n";
            else if (op == "JMPIF") out << "    if (memory[stoll(\"" << instr[1] << "\")] == memory[stoll(\"" << instr[2] << "\")]) {" << handleFunc(instr[3].substr(1, instr[3].length() - 2))  << "(); return;}\n";
            else if (op == "SOUT") {
                out << "    for (long long i = stoll(\"" << instr[1] << "\"); i < stoll(\"" << instr[1] << "\") + memory[stoll(\"" << instr[2] << "\")]; ++i)\n";
                out << "        std::cout << (char)memory[i];\n";
                out << "    std::cout << std::endl;\n";
            }
            else if (op == "SIN") {
                out << "    for (int i = 0; i < memory[stoll(\"" << instr[1] << "\")]; ++i) {\n";
                out << "        char c;\n";
                out << "        std::cin.get(c);\n";
                out << "        memory[stoll(\"" << instr[2] << "\") + i] = (long long)c;\n";
                out << "    }\n";
            }
            else if (op == "RIN") {
                out << "    size_t write_index = stoll(\"" << instr[1] << "\");\n";
                out << "    for (size_t i = 0; i < args.size(); ++i) {\n";
                out << "        for (size_t j = 0; j < args[i].size(); ++j) {\n";
                out << "            memory[write_index++] = args[i][j];\n";
                out << "        }\n";
                out << "        if (i + 1 < args.size()) memory[write_index++] = ' ';\n";
                out << "    }\n";
                out << "    memory[stoll(\"" << instr[2] << "\")] = write_index - stoll(\"" << instr[1] << "\");\n";
            }
            else if (op == "HALT") {
                out << "    exit(" << instr[1] << ");\n";
            }
        }

        out << "    return;\n}\n";
    }

    out << "int main(int argc, char** argv) {\n";
    out << "    for (int i = 1; i < argc; ++i) args.emplace_back(argv[i]);\n";
    out << "    progMain();\n    return 0;\n";
    out << "}\n";

    out.close();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input.sasm>\n";
        return 1;
    }

    std::ifstream inFile(argv[1]);
    if (!inFile) {
        std::cerr << "Failed to open file: " << argv[1] << "\n";
        return 1;
    }

    std::ostringstream buffer;
    buffer << inFile.rdbuf();
    auto tokens = tokenize(buffer.str());

    std::string outCpp = "output.cpp";
    generateCpp(tokens, outCpp);

    std::string command = "g++ -o sasmOut " + outCpp;
    if (system(command.c_str()) != 0) {
        std::cerr << "Compilation failed.\n";
        return 1;
    }

    remove(outCpp.c_str());

    std::cout << "Compilation successful. Run with: ./sasmOut\n";
    return 0;
}
