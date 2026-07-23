//
// Created by Admin on 23.07.2026.
//
#include "Token.h"
#include "AST.h"
#include <vector>
#include <string>

#ifndef INTERPRETER_ANNA_MOHIL_PARSER_H
#define INTERPRETER_ANNA_MOHIL_PARSER_H

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    StatementPtr parseStatement();
private:
    std::vector<Token> tokens;
    size_t pos = 0;
    const Token& current() const;
    const Token& advance();
    bool check(TokenType type) const;
    const Token& expect(TokenType type, const std::string& errorMsg);

    StatementPtr parseVarDecl();
    StatementPtr parseFuncDef();

    ExprPtr parseExpr();
    ExprPtr parseTerm();
    ExprPtr parseFactor();
    std::vector<ExprPtr> parseArgList();
};


#endif //INTERPRETER_ANNA_MOHIL_PARSER_H
