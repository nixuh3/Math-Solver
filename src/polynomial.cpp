#include "pch.h"
#include "polynomial.h"
#include "expression.h"
#include "utils.h"

Polynomial::Polynomial(Rational constant) { m_coefficients[0] = constant; }

Polynomial::Polynomial(int exponent, Rational coefficient) {
    m_coefficients[exponent] = coefficient;
}

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
                    case CARET: return Pow(left, right);
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

int Polynomial::Degree() const {
    if (m_coefficients.empty()) {
        return -1;
    }
    return m_coefficients.rbegin()->first;
}

Rational Polynomial::GetCoeff(int degree) const {
    auto it = m_coefficients.find(degree);
    if (it != m_coefficients.end()) {
        return it->second;
    }
    return {};
}

std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
    if (p.m_coefficients.empty()) {
        os << "0";
        return os;
    }

    bool isFirst = true;

    for (auto it = p.m_coefficients.rbegin(); it != p.m_coefficients.rend(); ++it) {
        int exponent = it->first;
        Rational coeff = it->second;

        if (isFirst) {
            if (coeff < 0) {
                os << "-";
                coeff = -coeff;
            }
            isFirst = false;
        } else {
            if (coeff < 0) {
                os << " - ";
                coeff = -coeff;
            } else {
                os << " + ";
            }
        }

        if (exponent == 0 || !(coeff == 1)) {
            os << coeff;
        }

        if (exponent > 0) {
            os << "x";
            if (exponent > 1) {
                os << "^" << exponent;
            }
        }
    }

    return os;
}

Polynomial Polynomial::operator-() const {
    Polynomial result;
    for (const auto& [key, value] : m_coefficients) {
        result.m_coefficients[key] = -value;
    }
    return result;
}

Polynomial Polynomial::operator+(const Polynomial& rhs) const {
    Polynomial result = *this;
    for (auto const& [key, value] : rhs.m_coefficients) {
        result.m_coefficients[key] += value;
    }
    result.normalize();
    return result;
}

Polynomial Polynomial::operator-(const Polynomial& rhs) const {
    Polynomial result = *this;
    for (auto const& [key, value] : rhs.m_coefficients) {
        result.m_coefficients[key] -= value;
    }
    result.normalize();
    return result;
}

Polynomial Polynomial::operator*(const Polynomial& rhs) const {
    Polynomial result;
    for (const auto& [key, value] : m_coefficients) {
        for (const auto& [rKey, rValue] : rhs.m_coefficients) {
            result.m_coefficients[key + rKey] += value * rValue;
        }
    }
    result.normalize();
    return result;
}

Polynomial Polynomial::operator/(const Polynomial& rhs) const {
    assert(rhs.Degree() == 0 && "Variable in denomator");
    assert(rhs.m_coefficients.at(0) != 0 && "Division by zero");

    Polynomial result;
    for (const auto& [key, value] : m_coefficients) {
        result.m_coefficients[key] = value / rhs.m_coefficients.at(0);
    }
    result.normalize();
    return result;
}

Polynomial Polynomial::Pow(Polynomial base, const Polynomial& exponent) {
    assert(exponent.Degree() == 0 && "Variable in exponent");
    assert(exponent.m_coefficients.at(0) >= 0 && "Negative exponenet");
    assert(exponent.m_coefficients.at(0).IsInteger() && "Decimal exponenet");

    int exp = static_cast<int>(exponent.m_coefficients.at(0).GetValue());
    Polynomial result(1);

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
        if (it->second == 0) {
            it = m_coefficients.erase(it);
        } else {
            ++it;
        }
    }
}
