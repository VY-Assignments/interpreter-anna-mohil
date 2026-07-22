//
// Created by Admin on 22.07.2026.
//

#include "Tokenizer.h"
#include <stdexpect>
#include <cctype>

#include "Token.h"


Tokenizer::Tokenizer(const std::string &source): src(source) {}

char Tokenizer::peek() const {
    if (pos >= src.size()) return '\0';
    return src[pos];
}

char Tokenizer::advance() {
    return src[pos++];
}

void Tokenizer::skipWhiteSpace() {
    while (pos < src.size() && std::isspace(static_cast<unsigned char>(peek()))) {
        advance();
    }
}

Token Tokenizer::readNumber() {
    size_t start = pos;
    while (pos < src.size() && (std::isdigit(static_cast<unsigned char>(peek())) || peek() == '.')) {
        advance();
    }
    std::string text = src.substr(start, pos - start);
    return Token(TokenType::NUMBER, text, std::stod(text));
}

Token Tokenizer::readIdent() {
    size_t start = pos;
    while (pos < src.size() && (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_')) {
        advance();
    }
    std::string text = src.substr(start, pos - start);
    return Token(TokenType::IDENTIFICATOR, text)
}

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;

    while (pos < src.size()) {
        skipWhiteSpace();
        if (pos >= src.size()) break;

        char c = peek();

        if (std::isdigit(static_cast<unsigned char>(c))) {
            tokens.push_back(readNumber());
            continue;
        }

        if (std::isalpha(static_cast<unsigned char>(c))) {
            tokens.push_back(readIdent());
            continue;
        }

        switch (c) {
            case '+': advance(); tokens.emplace_back(TokenType::PLUS, "+"); break;
            case '-': advance(); tokens.emplace_back(TokenType::MINUS, "-"); break;
            case '*': advance(); tokens.emplace_back(TokenType::STAR, "*"); break;
            case '=': advance(); tokens.emplace_back(TokenType::EQUAL, "="); break;
            case '/': advance(); tokens.emplace_back(TokenType::SLASH, "/"); break;
            case '(': advance(); tokens.emplace_back(TokenType::LEFTPAREN, "("); break;
            case ')': advance(); tokens.emplace_back(TokenType::RIGHTPAREN, ")"); break;
            case '{': advance(); tokens.emplace_back(TokenType::LEFTBRACE, "{"); break;
            case '}': advance(); tokens.emplace_back(TokenType::RIGHTBRACE, "}"); break;
            case ',': advance(); tokens.emplace_back(TokenType::COMMA, ","); break;
            default:
                throw std::runtime_error(std::string("Unexpected char: ") + c);
        }
    }

    tokens.emplace_back(TokenType::END, "");
    return tokens;
}
