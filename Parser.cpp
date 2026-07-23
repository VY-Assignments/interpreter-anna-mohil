//
// Created by Admin on 23.07.2026.
//

#include "Parser.h"
#include <stdexcept>

Parser::Parser(std::vector<Token> tokens): tokens(std::move(tokens)) {}

const Token& Parser::current() const {
    return tokens[pos];
}

const Token& Parser::advance() {
    const Token& t = tokens[pos];
    if (pos + 1 < tokens.size()) {
        pos++;
        return t;
    }
}

bool Parser::check(TokenType type) const {
    return current().type == type;
}

const Token& Parser::expect(TokenType type, const std::string& errorMsg) {
    if (!check(type)) {
        throw std::runtime_error(errorMsg);
    }
    return advance();
}

StatementPtr Parser::parseStatement() {
    if (check(TokenType::IDENTIFICATOR) && current().text == "var") {
        return parseVarDecl();
    }
    if (check(TokenType::IDENTIFICATOR) && current().text == "def") {
        return parseFuncDef();
    }
    ExprPtr expr =parseExpr();
    expect(TokenType::END, "expected end of statement");
    return std::make_shared<ExprStatement>(expr);
}

StatementPtr Parser::parseVarDecl() {
    advance();
    Token name_token = expect(TokenType::IDENTIFICATOR, "expected variable name after 'var'");
    expect(TokenType::EQUAL, "expected '=");
    ExprPtr value = parseExpr();
    expect(TokenType::END, "expected end of statement");
    return std::make_shared<VarDeclStatement>(name_token.text, value);
}

StatementPtr Parser::parseFuncDef() {
    advance();
    Token name_token = expect(TokenType::IDENTIFICATOR, "expected variable name after 'def'");
    expect(TokenType::LEFTPAREN, "expected '('");

    std::vector<std::string> parameters;
    if (!check(TokenType::RIGHTPAREN)) {
        Token p = expect(TokenType::IDENTIFICATOR, "expected parameter");
        parameters.push_back(p.text);
        while (check(TokenType::COMMA)) {
            advance();
            Token p2 = expect(TokenType::IDENTIFICATOR, "expected parameter");
            parameters.push_back(p2.text);
        }
    }
    expect(TokenType::RIGHTPAREN, "expected ')'");
    expect(TokenType::LEFTBRACE, "expected '{'");
    ExprPtr body = parseExpr();
    expect(TokenType::RIGHTBRACE, "expected '}'");
    expect(TokenType::END, "expected end of statement");
    return std::make_shared<FuncDefStatement>(name_token.text, parameters, body);
}

ExprPtr Parser::parseExpr() {
    ExprPtr left = parseTerm();
    while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
        char op = current().text[0];
        advance();
        ExprPtr right = parseTerm();
        left = std::make_shared<BinaryExpr>(op, left, right);
    }
    return left;
}

ExprPtr Parser::parseTerm() {
    ExprPtr left = parseFactor();
    while (check(TokenType::STAR) || check(TokenType::SLASH)) {
        char op = current().text[0];
        advance();
        ExprPtr right = parseFactor();
        left = std::make_shared<BinaryExpr>(op, left, right);
    }
    return left;
}

ExprPtr Parser::parseFactor() {
    if (check(TokenType::NUMBER)) {
        Tokenb t = advance();
        return std::make_shared<NumExpr>(t.number);
    }

    if (check(TokenType::MINUS)) {
        advance();
        ExprPtr inner = parseFactor();
        return std::make_shared<UnaryMinusExpr>(inner);
    }

    if (check(TokenType::LEFTPAREN)) {
        advance();
        ExprPtr inner = parseExpr();
        expect(TokenType::RIGHTPAREN, "expected ')'");
        return inner;
    }

    if (check(TokenType::IDENTIFICATOR)) {
        Token token_name = advance();
        if (check(TokenType::LEFTPAREN)) {
            advance();
            std::vector<ExprPtr> args = parseArgList();
            expect(TokenType::RIGHTPAREN, "expected ')'");
            return std::make_shared<CallExpr>(token_name.text, args);
        }
        return std::make_shared<IdentExpr>(token_name.text);
    }
    throw std::runtime_error("unexpected token");
}

std::vector<ExprPtr> Parser::parseArgList() {
    std::vector<ExprPtr> args;
    if (check(TokenType::RIGHTPAREN)) return args;
    args.push_back(parseExpr());
    while (check(TokenType::COMMA)) {
        advance();
        args.push_back(parseExpr());
    }
    return args;
}
