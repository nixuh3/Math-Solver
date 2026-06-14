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

    int Degree() const;
    Rational GetCoeff(int degree) const;

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p);

    Polynomial operator-() const;
    Polynomial operator+(const Polynomial& rhs) const;
    Polynomial operator-(const Polynomial& rhs) const;
    Polynomial operator*(const Polynomial& rhs) const;
    Polynomial operator/(const Polynomial& rhs) const;
    static Polynomial Pow(Polynomial base, const Polynomial& exponent);

  private:
    void normalize();

    std::map<int, Rational> m_coefficients;
};
