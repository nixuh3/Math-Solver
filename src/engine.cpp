#include "pch.h"
#include "engine.h"
#include "expression.h"
#include "arena.h"

#include "ast_printer.h"

Engine::Engine(const Expr* expr, Arena& arena) : m_arena(arena), m_expr(expr) {}

double Engine::Evaluate() {
    std::cout << "Before: " << AstPrinter::Print(m_expr) << "\n";
    m_expr = simplify(m_expr);
    std::cout << "After: " << AstPrinter::Print(m_expr) << "\n";

    return evaluateExpr(m_expr);
}

double Engine::evaluateExpr(const Expr* expr) {
    return std::visit([this](auto&& node) -> double { return visit(node); }, *expr);
}

const Expr* Engine::simplify(const Expr* expr) {
    return std::visit(
        [this, expr](auto&& node) -> const Expr* {
            using T = std::decay_t<decltype(node)>;

            if constexpr (std::is_same_v<T, Binary>) {
                const Expr* newLeft = simplify(node.left);
                const Expr* newRight = simplify(node.right);

                if (std::holds_alternative<Literal>(*newLeft) &&
                    std::holds_alternative<Literal>(*newRight)) {
                    return m_arena.Alloc<Expr>(Literal{ visit({ newLeft, node.op, newRight }) });
                }

                if (newLeft != node.left || newRight != node.right) {
                    return m_arena.Alloc<Expr>(Binary{ newLeft, node.op, newRight });
                }
            } else if constexpr (std::is_same_v<T, Unary>) {
                const Expr* newRight = simplify(node.right);

                if (std::holds_alternative<Literal>(*newRight)) {
                    return m_arena.Alloc<Expr>(Literal{ visit({ node.op, newRight }) });
                }

                if (newRight != node.right) {
                    return m_arena.Alloc<Expr>(Unary{ node.op, newRight });
                }
            } else if constexpr (std::is_same_v<T, Grouping>) {
                const Expr* newExpr = simplify(node.expression);

                if (std::holds_alternative<Literal>(*newExpr)) {
                    return newExpr;
                }

                if (newExpr != node.expression) {
                    return m_arena.Alloc<Expr>(Grouping{ newExpr });
                }
            }

            return expr;
        },
        *expr);
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
