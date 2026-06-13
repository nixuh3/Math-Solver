#pragma once

#include <map>
#include <string>

struct Expr;

class Polynomial {
  public:
    Polynomial() = default;
    Polynomial(double constant);
    Polynomial(int exponent, double coefficient);

    static Polynomial FromExpr(const Expr* expr);
    void Print() const;

  private:
    double& operator[](int key) { return m_coefficients[key]; }
    double operator[](int exponent) const { return at(exponent); }

    double at(int exponent) const {
        auto it = m_coefficients.find(exponent);
        return (it != m_coefficients.end()) ? it->second : 0.0;
    }

    Polynomial operator-() const;
    Polynomial operator+(const Polynomial& rhs) const;
    Polynomial operator-(const Polynomial& rhs) const;
    Polynomial operator*(const Polynomial& rhs) const;
    Polynomial operator/(const Polynomial& rhs) const;
    Polynomial operator^(const Polynomial& rhs) const;

    void normalize();
    int degree() const;

    std::map<int, double> m_coefficients;
};
