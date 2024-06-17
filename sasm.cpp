#include <iostream>
#include <vector>
#include <map>
#include <regex>

std::map<std::string, std::vector<std::vector<std::string>>> tokenize(const std::string& code) {
    std::map<std::string, std::vector<std::vector<std::string>>> tokens;
    std::string currentLabel;

    std::regex labelRegex(R"(<(\w+)>:)");

    std::istringstream iss(code);
    for (std::string line; std::getline(iss, line); ) {
        std::string noComments = std::regex_replace(line, std::regex("\\s*--.*"), "");
        std::string trimmed = std::regex_replace(noComments, std::regex("^\\s+|\\s+$"), "");

        if (trimmed.empty())
            continue;

        std::smatch match;
        if (std::regex_match(trimmed, match, labelRegex)) {
            currentLabel = match[1];
            currentLabel = "<" + currentLabel + ">";
            tokens[currentLabel] = std::vector<std::vector<std::string>>();
        }
        else {
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
                std::vector<std::string> args;
                for (std::string arg; std::getline(argumentsIss, arg, ','); ) {
                    args.push_back(arg);
                }
                tokenized.insert(tokenized.end(), args.begin(), args.end());
            }
            tokens[currentLabel].push_back(tokenized);
        }
    }

    return tokens;
}

class Interpreter {
private:
    std::map<long long, long long> memory;
    std::map<std::string, std::vector<std::vector<std::string>>> program;
    std::string args;

public:
    Interpreter(const std::map<std::string, std::vector<std::vector<std::string>>>& tokenizedProgram, const std::string& arguments) : program(tokenizedProgram), args(arguments) {}

    void interpret() {
        std::vector<std::vector<std::string>> mainBlock = program["<main>"];

        executeBlock(mainBlock);
    }

    void executeBlock(const std::vector<std::vector<std::string>>& instructionsList) {
        bool stop = false;
        for (const auto& instructions : instructionsList) {
            std::string instruction = instructions[0];
            std::vector<std::string> arguments(instructions.begin() + 1, instructions.end());

            if (stop) {
                break;
            }

            if (instruction == "ADD") {
                memory[std::stoll(arguments[2])] = memory[std::stoll(arguments[0])] + memory[std::stoll(arguments[1])];
            } else if (instruction == "SUB") {
                memory[std::stoll(arguments[2])] = memory[std::stoll(arguments[0])] - memory[std::stoll(arguments[1])];
            } else if (instruction == "MUL") {
                memory[std::stoll(arguments[2])] = memory[std::stoll(arguments[0])] * memory[std::stoll(arguments[1])];
            } else if (instruction == "DIV") {
                memory[std::stoll(arguments[2])] = memory[std::stoll(arguments[0])] / memory[std::stoll(arguments[1])];
            } else if (instruction == "REM") {
                memory[std::stoll(arguments[2])] = memory[std::stoll(arguments[0])] % memory[std::stoll(arguments[1])];
            } else if (instruction == "STR") {
                memory[std::stoll(arguments[1])] = std::stoll(arguments[0]);
            } else if (instruction == "CPY") {
                memory[std::stoll(arguments[1])] = memory[std::stoll(arguments[0])];
            } else if (instruction == "AND") {
                memory[std::stoll(arguments[2])] = memory[std::stoll(arguments[0])] & memory[std::stoll(arguments[1])];
            } else if (instruction == "OR") {
                memory[std::stoll(arguments[2])] = memory[std::stoll(arguments[0])] | memory[std::stoll(arguments[1])];
            } else if (instruction == "XOR") {
                memory[std::stoll(arguments[2])] = memory[std::stoll(arguments[0])] ^ memory[std::stoll(arguments[1])];
            } else if (instruction == "NOT") {
                memory[std::stoll(arguments[1])] = ~memory[std::stoll(arguments[0])];
            } else if (instruction == "JMP") {
                executeBlock(program[arguments[0]]);
                stop = true;
            } else if (instruction == "JMPBZ") {
                if (memory[std::stoll(arguments[0])] > 0) {
                    executeBlock(program[arguments[1]]);
                    stop = true;
                }
            } else if (instruction == "JMPZ") {
                if (memory[std::stoll(arguments[0])] == 0L) {
                    executeBlock(program[arguments[1]]);
                    stop = true;
                }
            } else if (instruction == "JMPIF") {
                if (memory[std::stoll(arguments[0])] == memory[std::stoll(arguments[1])]) {
                    executeBlock(program[arguments[2]]);
                    stop = true;
                }
            } else if (instruction == "SOUT") {
                long long start = std::stoll(arguments[0]);
                long long length = memory[std::stoll(arguments[1])];
                std::string str;

                for (long long i = start; i < start + length; ++i) {
                    str += static_cast<char>(memory[i]);
                }

                std::cout << str << std::endl;
            } else if (instruction == "SIN") {
                long long length = std::stoll(arguments[0]);
                std::string str;

                for (long long i = 0; i < length; ++i) {
                    char c = static_cast<char>(getchar());
                    str += c;
                }

                long long start = std::stoll(arguments[1]);
                for (long long i = 0; i < length; ++i) {
                    memory[start + i] = static_cast<long long>(str[i]);
                }
            } else if (instruction == "RIN") {
               int i2 = 0;
               std::vector<char> charArray(args.begin(), args.end());
               std::vector<int> asciiArray;

               for (char c : charArray) {
                   asciiArray.push_back(static_cast<int>(c));
               }

               for (int asciiValue : asciiArray) {
                   memory[std::stoll(arguments[0]) + i2] = static_cast<long long>(asciiValue);
                   i2++;
               }

               memory[std::stoll(arguments[1])] = static_cast<long long>(i2);
            } else if (instruction == "HALT") {
                std::cout << "Program exited with code " << arguments[0] << std::endl;
                stop = true;
            } else {
                std::cerr << "Unknown instruction: " << instruction << std::endl;
                stop = true;
            }
        }
    }
};

#include <fstream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <program_file> [program_arguments]" << std::endl;
        return 0;
    }

    std::string programFile = argv[1];
    std::string programArgs;

    for (int i = 2; i < argc; ++i) {
        programArgs += argv[i];
        if (i < argc - 1) {
            programArgs += ' ';
        }
    }

    std::ifstream file(programFile);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string programCode = buffer.str();
        file.close();

        std::map<std::string, std::vector<std::vector<std::string>>> tokenized = tokenize(programCode);

        Interpreter interpreter(tokenized, programArgs);
        interpreter.interpret();
    } else {
        std::cerr << "Failed to open file: " << programFile << std::endl;
        return 1;
    }

    return 0;
}
