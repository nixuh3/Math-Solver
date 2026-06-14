#include "pch.h"
#include "ast_printer.h"
#include "utils.h"
#include "expression.h"

std::string AstPrinter::Print(const Equation* equation) {
    m_ss.str("");

    printExpr(equation->left);
    m_ss << " = ";
    printExpr(equation->right);
    return m_ss.str();
}

void AstPrinter::printExpr(const Expr* expr) {
    std::visit([](auto&& node) { visit(node); }, *expr);
}

void AstPrinter::visit(const Binary& expr) {
    printExpr(expr.left);
    m_ss << " " << OperatorToStr(expr.op.type) << " ";
    printExpr(expr.right);
}

void AstPrinter::visit(const Unary& expr) {
    m_ss << OperatorToStr(expr.op.type);
    printExpr(expr.right);
}

void AstPrinter::visit(const Grouping& expr) {
    m_ss << "(";
    printExpr(expr.expression);
    m_ss << ")";
}

void AstPrinter::visit(const Literal& expr) { m_ss << expr.value; }

void AstPrinter::visit(const Variable& expr) { m_ss << expr.name; }
