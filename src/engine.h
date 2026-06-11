#pragma once

struct Expr;
struct Binary;
struct Unary;
struct Grouping;
struct Literal;
struct Variable;

class Engine {
  public:
    explicit Engine(const Expr* expr);
    double Evaluate();

  private:
    double evaluateExpr(const Expr* expr);

    double visit(const Binary& expr);
    double visit(const Unary& expr);
    double visit(const Grouping& expr);
    double visit(const Literal& expr);
    double visit(const Variable& expr);

  private:
    const Expr* m_expr;
};
