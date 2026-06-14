#pragma once

#include "rational.h"
#include <string>

enum TokenType {
    LEFT_PAREN,
    RIGHT_PAREN,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    CARET, // ^
    EQUAL,
    IDENTIFIER,
    NUMBER,
    END
};

constexpr std::string OperatorToStr(TokenType type) {
    switch (type) {
        case LEFT_PAREN: return "(";
        case RIGHT_PAREN: return ")";
        case PLUS: return "+";
        case MINUS: return "-";
        case STAR: return "*";
        case SLASH: return "/";
        case CARET: return "^";
        case EQUAL: return "=";
        default: return "";
    }
}

struct Token {
    Token(TokenType type, std::string_view lexeme, Rational value, int line)
        : type(type), lexeme(lexeme), literal(value), line(line) {}

    const TokenType type;
    const std::string lexeme;
    const Rational literal;
    const int line;
};
