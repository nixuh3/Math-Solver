#include "pch.h"
#include "engine.h"
#include "expression.h"
#include "arena.h"
#include "polynomial.h"
#include "ast_printer.h"

Engine::Engine(const Equation* equation, Arena& arena) : m_arena(arena), m_equation(equation) {}

Rational Engine::Evaluate() {
    std::cout << "Before: " << AstPrinter::Print(m_equation) << "\n";

    const Expr* simplifiedLeft = simplify(m_equation->left);
    const Expr* simplifiedRight = simplify(m_equation->right);

    const Equation* simplifiedEquation = m_equation;
    if (simplifiedLeft != m_equation->left || simplifiedRight != m_equation->right) {
        simplifiedEquation = m_arena.Alloc<Equation>(Equation{ simplifiedLeft, simplifiedRight });
    }

    std::cout << "After: " << AstPrinter::Print(simplifiedEquation) << "\n";

    Polynomial leftPoly = Polynomial::FromExpr(simplifiedEquation->left);
    Polynomial rightPoly = Polynomial::FromExpr(simplifiedEquation->right);
    leftPoly.Print();
    std::cout << " = ";
    rightPoly.Print();
    std::cout << "\n";

    // TODO: solve the equation
    return 0;
}

Rational Engine::evaluateExpr(const Expr* expr) {
    return std::visit([this](auto&& node) -> Rational { return visit(node); }, *expr);
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

Rational Engine::visit(const Binary& expr) {
    Rational left = evaluateExpr(expr.left);
    Rational right = evaluateExpr(expr.right);

    switch (expr.op.type) {
        case PLUS: return left + right;
        case MINUS: return left - right;
        case STAR: return left * right;
        case SLASH: return left / right;
        case CARET: return left ^ right;
        default: break;
    }

    return {};
}

Rational Engine::visit(const Unary& expr) {
    Rational right = evaluateExpr(expr.right);

    switch (expr.op.type) {
        case MINUS: return -right;
        case PLUS: return right;
        default: break;
    }

    return {};
}

Rational Engine::visit(const Grouping& expr) { return evaluateExpr(expr.expression); }

Rational Engine::visit(const Literal& expr) { return expr.value; }

Rational Engine::visit(const Variable& expr) { return {}; }
