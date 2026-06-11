#include "token.h"

Token::Token(TokenType type, std::string_view lexeme, double value, int line)
    : type(type), lexeme(lexeme), literal(value), line(line) {}
