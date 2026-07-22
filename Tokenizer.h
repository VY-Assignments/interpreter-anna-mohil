//
// Created by Admin on 22.07.2026.
//

#include <vector>
#include <string>

#ifndef INTERPRETER_ANNA_MOHIL_TOKENIZER_H
#define INTERPRETER_ANNA_MOHIL_TOKENIZER_H


class Tokenizer {
public:
    explicit Tokenizer(const std::string& source);
    std::vector<Token> tokenize();

private:
    const std::string& src;
    size_t pos = 0;

    char peek() const;
    char advance();
    void skipWhiteSpace();
    Token readNumber();
    Token readIdent();
    

};


#endif //INTERPRETER_ANNA_MOHIL_TOKENIZER_H
