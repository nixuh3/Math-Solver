#pragma once

#include "rational.h"
#include <map>
#include <string>

struct Expr;

class Polynomial {
  public:
    Polynomial() = default;
    Polynomial(Rational constant);
    Polynomial(int exponent, Rational coefficient);

    static Polynomial FromExpr(const Expr* expr);
    void Print() const;

  private:
    Polynomial operator-() const;
    Polynomial operator+(const Polynomial& rhs) const;
    Polynomial operator-(const Polynomial& rhs) const;
    Polynomial operator*(const Polynomial& rhs) const;
    Polynomial operator/(const Polynomial& rhs) const;
    Polynomial operator^(const Polynomial& rhs) const;

    void normalize();
    int degree() const;

    std::map<int, Rational> m_coefficients;
};
