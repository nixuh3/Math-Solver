#pragma once

#include "expression.h"
#include "arena.h"
#include <vector>

class Parser {
  public:
    Parser(const std::vector<Token>& tokens, Arena& arena);
    const Expr* Parse();

  private:
    struct ParseError {};

    const Expr* expression();
    const Expr* term();
    const Expr* factor();
    const Expr* unary();
    const Expr* primary();

    template <typename... Args>
    bool match(Args... args);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    Token consume(TokenType type, std::string_view message);
    ParseError error(Token token, std::string_view message);

    const std::vector<Token>& m_tokens;
    int m_current = 0;
    Arena& m_arena;
};
