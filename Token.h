//
// Created by Admin on 22.07.2026.
//
#include <string>
#ifndef INTERPRETER_ANNA_MOHIL_TOKEN_H
#define INTERPRETER_ANNA_MOHIL_TOKEN_H


enum class TokenType
{
    NUMBER,
    IDENTIFICATOR,
    PLUS, MINUS, STAR, SLASH,
    EQUAL,
    COMMA,
    LEFTPAREN, RIGHTPAREN,
    LEFTBRACE, RIGHTBRACE,
    END
};

struct Token {
    TokenType token_type;
    std::string text;
    double number = 0;

    Token(TokenType token, std::string txt = "", double num = 0): token_type(token), text(std::move(txt)), number(num) {}
};

#endif //INTERPRETER_ANNA_MOHIL_TOKEN_H
