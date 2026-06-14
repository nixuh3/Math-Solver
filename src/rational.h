#pragma once

#include <cstdint>
#include <iosfwd>

class Rational {
  public:
    Rational(int64_t n = 0, int64_t d = 1);
    explicit Rational(double value);

    bool IsInteger() const;
    double GetValue() const;

    Rational operator-() const;

    Rational& operator+=(const Rational& rhs);
    Rational& operator-=(const Rational& rhs);
    Rational& operator*=(const Rational& rhs);
    Rational& operator/=(const Rational& rhs);

    friend Rational operator+(Rational lhs, const Rational& rhs);
    friend Rational operator-(Rational lhs, const Rational& rhs);
    friend Rational operator*(Rational lhs, const Rational& rhs);
    friend Rational operator/(Rational lhs, const Rational& rhs);
    static Rational Pow(const Rational& base, const Rational& exp);

    bool operator==(const Rational& rhs) const;
    bool operator!=(const Rational& rhs) const;
    bool operator<(const Rational& rhs) const;
    bool operator>(const Rational& rhs) const;
    bool operator<=(const Rational& rhs) const;
    bool operator>=(const Rational& rhs) const;

    friend std::ostream& operator<<(std::ostream& os, const Rational& r);

  private:
    void normalize();
    void assignNormalized(__int128_t n, __int128_t d);

    int64_t m_num = 0;
    int64_t m_den = 1; // always > 0
};
