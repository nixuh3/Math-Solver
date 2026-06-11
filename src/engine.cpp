#include "engine.h"
#include "expression.h"
#include <cmath>

Engine::Engine(const Expr* expr) : m_expr(expr) {}

double Engine::Evaluate() { return evaluateExpr(m_expr); }

double Engine::evaluateExpr(const Expr* expr) {
    return std::visit([this](auto&& node) { return visit(node); }, *expr);
}

double Engine::visit(const Binary& expr) {
    double left = evaluateExpr(expr.left);
    double right = evaluateExpr(expr.right);

    switch (expr.op.type) {
        case PLUS: return left + right;
        case MINUS: return left - right;
        case STAR: return left * right;
        case SLASH: return left / right;
        case CARET: return std::pow(left, right);
        default: break;
    }

    return 0.0;
}

double Engine::visit(const Unary& expr) {
    double right = evaluateExpr(expr.right);

    switch (expr.op.type) {
        case MINUS: return -right;
        case PLUS: return right;
        default: break;
    }

    return 0.0;
}

double Engine::visit(const Grouping& expr) { return evaluateExpr(expr.expression); }

double Engine::visit(const Literal& expr) { return expr.value; }

double Engine::visit(const Variable& expr) { return 0.0; }
