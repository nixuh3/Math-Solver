#include "pch.h"
#include "engine.h"
#include "expression.h"
#include "arena.h"
#include "polynomial.h"
#include "ast_printer.h"

Engine::Engine(const Equation* equation, Arena& arena) : m_arena(arena), m_equation(equation) {}

Engine::Roots Engine::Evaluate() {
    std::cout << "Start: " << AstPrinter::Print(m_equation) << "\n";

    const Expr* simplifiedLeft = simplify(m_equation->left);
    const Expr* simplifiedRight = simplify(m_equation->right);

    const Equation* simplifiedEquation = m_equation;
    if (simplifiedLeft != m_equation->left || simplifiedRight != m_equation->right) {
        simplifiedEquation = m_arena.Alloc<Equation>(simplifiedLeft, simplifiedRight);
    }

    std::cout << "Simplified: " << AstPrinter::Print(simplifiedEquation) << "\n";

    Polynomial leftPoly = Polynomial::FromExpr(simplifiedEquation->left);
    Polynomial rightPoly = Polynomial::FromExpr(simplifiedEquation->right);
    std::cout << "Polynomial form: " << leftPoly << " = " << rightPoly << "\n";

    Polynomial lhs = leftPoly - rightPoly;
    std::cout << "Bring terms to the lhs: " << lhs << " = 0\n";

    Roots roots;
    switch (lhs.Degree()) {
        case 0:
            std::cout << "No variables\n";
            if (lhs.GetCoeff(0) == 0) {
                roots.isInfinite = true;
            } else {
                roots.isNone = true;
            }
            break;
        case 1:
            std::cout << "Linear\n";
            // ax + b = 0 -> x = -b/a
            roots.roots.emplace_back(-lhs.GetCoeff(0) / lhs.GetCoeff(1));
            break;
        case 2: {
            std::cout << "Quadratic\n";

            Rational c = lhs.GetCoeff(0);
            Rational b = lhs.GetCoeff(1);
            Rational a = lhs.GetCoeff(2);

            Rational delta = Rational::Pow(b, 2) - 4 * a * c;

            if (delta < 0) {
                roots.isNone = true;
            } else if (delta == 0) {
                roots.roots.emplace_back(-b / (2 * a));
            } else {
                // TODO
            }
            break;
        }
        default: std::cout << "Higher degree equations not supported\n"; break;
    }

    return roots;
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
        case CARET: return Rational::Pow(left, right);
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
