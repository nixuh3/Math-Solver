#pragma once

#include <sstream>

struct Expr;
struct Binary;
struct Unary;
struct Grouping;
struct Literal;
struct Variable;

class AstPrinter {
  public:
    AstPrinter() = delete;
    static std::string Print(const Expr* expr);

  private:
    static std::string printExpr(const Expr* expr);

    static void visit(const Binary& expr);
    static void visit(const Unary& expr);
    static void visit(const Grouping& expr);
    static void visit(const Literal& expr);
    static void visit(const Variable& expr);

  private:
    inline static std::stringstream m_ss;
};
