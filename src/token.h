#pragma once

#include <string>
#include <variant>

enum TokenType {
    LEFT_PAREN,
    RIGHT_PAREN,
    DOT,
    MINUS,
    PLUS,
    SLASH,
    STAR,
    EQUAL,
    IDENTIFIER,
    NUMBER,
    END
};

using Value = std::variant<std::monostate, double, std::string>;

std::string ValToStr(const Value& value);

struct Token {
    Token(TokenType type, std::string_view lexeme, const Value& value, int line);

    std::string ToStr() const;

    const TokenType type;
    const std::string lexeme;
    const Value literal;
    const int line;
};
