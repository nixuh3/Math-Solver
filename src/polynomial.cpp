#include "pch.h"
#include "polynomial.h"
#include "expression.h"
#include "utils.h"

Polynomial::Polynomial(double constant) { m_coefficients[0] = constant; }

Polynomial::Polynomial(int exponent, double coefficient) { m_coefficients[exponent] = coefficient; }

Polynomial Polynomial::FromExpr(const Expr* expr) {
    return std::visit(
        [](auto&& node) -> Polynomial {
            using T = std::decay_t<decltype(node)>;

            if constexpr (std::is_same_v<T, Literal>) {
                return Polynomial(node.value);
            } else if constexpr (std::is_same_v<T, Grouping>) {
                return FromExpr(node.expression);
            } else if constexpr (std::is_same_v<T, Variable>) {
                return Polynomial(1, 1.0);
            } else if constexpr (std::is_same_v<T, Binary>) {
                Polynomial left = FromExpr(node.left);
                Polynomial right = FromExpr(node.right);

                switch (node.op.type) {
                    case PLUS: return left + right;
                    case MINUS: return left - right;
                    case STAR: return left * right;
                    case SLASH: return left / right;
                    case CARET: return left ^ right;
                    default: assert(false && "Unsupported binary operator");
                }
            } else if constexpr (std::is_same_v<T, Unary>) {
                Polynomial right = FromExpr(node.right);

                switch (node.op.type) {
                    case PLUS: return right;
                    case MINUS: return -right;
                    default: assert(false && "Unsupported unary operator");
                }
            }
        },
        *expr);
}

void Polynomial::Print() const {
    for (auto it = m_coefficients.rbegin(); it != m_coefficients.rend(); ++it) {
        if (it->first > 0) {
            std::cout << " + " << (it->second == 1.0 ? FormatDouble(it->second) : "") << "x^"
                      << it->first;
        } else {
            std::cout << " + " << it->second;
        }
    }
    std::cout << "\n";
}

Polynomial Polynomial::operator-() const {
    Polynomial result;
    for (const auto& [key, value] : m_coefficients) {
        result[key] = -value;
    }
    return result;
}

Polynomial Polynomial::operator+(const Polynomial& rhs) const {
    Polynomial result = *this;
    for (auto const& [key, value] : rhs.m_coefficients) {
        result[key] += value;
    }
    result.normalize();
    return result;
}

Polynomial Polynomial::operator-(const Polynomial& rhs) const {
    Polynomial result = *this;
    for (auto const& [key, value] : rhs.m_coefficients) {
        result[key] -= value;
    }
    result.normalize();
    return result;
}

Polynomial Polynomial::operator*(const Polynomial& rhs) const {
    Polynomial result;
    for (const auto& [key, value] : m_coefficients) {
        for (const auto& [rKey, rValue] : rhs.m_coefficients) {
            result[key + rKey] += value * rValue;
        }
    }
    result.normalize();
    return result;
}

Polynomial Polynomial::operator/(const Polynomial& rhs) const {
    assert(rhs.degree() == 0 && "Variable in denomator");
    assert(rhs[0] != 0.0 && "Division by zero");

    Polynomial result;
    for (const auto& [key, value] : m_coefficients) {
        result[key] = value / rhs[0];
    }
    result.normalize();
    return result;
}

Polynomial Polynomial::operator^(const Polynomial& rhs) const {
    assert(rhs.degree() == 0 && "Variable in exponent");
    assert(rhs[0] >= 0 && "Negative exponenet");
    assert(std::floor(rhs[0]) == rhs[0] && "Decimal exponenet");

    Polynomial base = *this;
    int exp = static_cast<int>(rhs[0]);
    Polynomial result(1.0);

    while (exp > 0) {
        if (exp & 1) {
            result = result * base;
        }

        base = base * base;
        exp >>= 1;
    }

    result.normalize();
    return result;
}

void Polynomial::normalize() {
    for (auto it = m_coefficients.begin(); it != m_coefficients.end();) {
        if (std::abs(it->second) < 1e-12) {
            it = m_coefficients.erase(it);
        } else {
            ++it;
        }
    }
}

int Polynomial::degree() const {
    if (m_coefficients.empty()) {
        return -1;
    }
    return m_coefficients.rbegin()->first;
}
