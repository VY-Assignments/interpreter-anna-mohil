//
// Created by Admin on 23.07.2026.
//
#include "Parser.h"
#include "Environment.h"
#include <iostream>
#include <string>

#include "Tokenizer.h"

int main() {
    Environment globalEnv;
    std::string input;

    while (std::getline(std::cin,input)) {
        if (input == "exit" || input == "quit") break;
        if (input.empty()) continue;
        try {
            Tokenizer tokenizer(input);
            std::vector<Token> tokens = tokenizer.tokenize();

            Parser parser(tokens);
            StatementPtr statement = parser.parseStatement();

            statement->execute(globalEnv);
        }catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
    return 0;
}
