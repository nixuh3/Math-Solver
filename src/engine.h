#pragma once

#include "rational.h"

struct Equation;
struct Expr;
struct Binary;
struct Unary;
struct Grouping;
struct Literal;
struct Variable;

class Arena;

class Engine {
  public:
    explicit Engine(const Equation* equation, Arena& arena);
    Rational Evaluate();

  private:
    Rational evaluateExpr(const Expr* expr);
    const Expr* simplify(const Expr* expr);

    Rational visit(const Binary& expr);
    Rational visit(const Unary& expr);
    Rational visit(const Grouping& expr);
    Rational visit(const Literal& expr);
    Rational visit(const Variable& expr);

  private:
    const Equation* m_equation;
    Arena& m_arena;
};
