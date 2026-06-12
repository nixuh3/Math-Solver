#pragma once

#include "arena.h"
#include "token.h"
#include <vector>

struct Expr;
struct Equation;

class Parser {
  public:
    Parser(const std::vector<Token>& tokens, Arena& arena);
    const Equation* Parse();

  private:
    struct ParseError {};

    const Equation* equation();

    const Expr* term();
    const Expr* factor();
    const Expr* power();
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
