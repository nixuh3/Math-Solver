#pragma once

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
    double Evaluate();

  private:
    double evaluateExpr(const Expr* expr);
    const Expr* simplify(const Expr* expr);

    double visit(const Binary& expr);
    double visit(const Unary& expr);
    double visit(const Grouping& expr);
    double visit(const Literal& expr);
    double visit(const Variable& expr);

  private:
    const Equation* m_equation;
    Arena& m_arena;
};
