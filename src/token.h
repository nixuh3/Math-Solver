#pragma once

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

struct Token {
    Token(TokenType type, std::string_view lexeme, double value, int line);

    const TokenType type;
    const std::string lexeme;
    const double literal;
    const int line;
};
