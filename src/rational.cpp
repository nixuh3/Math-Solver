#include "pch.h"
#include "rational.h"

Rational::Rational(int64_t n, int64_t d) : m_num(n), m_den(d) {
    assert(m_den != 0 && "Denominator is zero");
    normalize();
}

Rational::Rational(double value) {
    if (std::isnan(value) || std::isinf(value)) {
        throw std::domain_error("Cannot convert NaN or Infinity to Rational");
    }

    int sign = (value < 0) ? -1 : 1;
    value = std::abs(value);

    const double error = 1e-9;
    double n = value;

    int64_t a = static_cast<int64_t>(std::floor(n));
    int64_t h0 = 0, h1 = 1, h2 = a;
    int64_t k0 = 1, k1 = 0, k2 = 1;

    while (std::abs(value - (static_cast<double>(h2) / k2)) > error) {
        if (std::abs(n - a) < 1e-12) {
            break; // division by zero
        }

        n = 1.0 / (n - a);
        a = static_cast<int64_t>(std::floor(n));

        h0 = h1;
        h1 = h2;
        k0 = k1;
        k1 = k2;

        // check for integer overflow
        if ((__int128_t)a * h1 + h0 > INT64_MAX || (__int128_t)a * k1 + k0 > INT64_MAX) {
            break;
        }

        h2 = a * h1 + h0;
        k2 = a * k1 + k0;
    }

    m_num = sign * h2;
    m_den = k2;
    normalize();
}

bool Rational::IsInteger() const { return m_num % m_den == 0; }

double Rational::GetValue() const { return static_cast<double>(m_num) / m_den; }

Rational Rational::operator-() const { return Rational(-m_num, m_den); }

Rational& Rational::operator+=(const Rational& rhs) {
    assignNormalized((__int128_t)m_num * rhs.m_den + (__int128_t)rhs.m_num * m_den,
        (__int128_t)m_den * rhs.m_den);
    return *this;
}

Rational& Rational::operator-=(const Rational& rhs) {
    assignNormalized((__int128_t)m_num * rhs.m_den - (__int128_t)rhs.m_num * m_den,
        (__int128_t)m_den * rhs.m_den);
    return *this;
}

Rational& Rational::operator*=(const Rational& rhs) {
    int64_t g1 = std::gcd(m_num, rhs.m_den);
    int64_t g2 = std::gcd(rhs.m_num, m_den);

    m_num = (m_num / g1) * (rhs.m_num / g2);
    m_den = (m_den / g2) * (rhs.m_den / g1);
    normalize();
    return *this;
}

Rational& Rational::operator/=(const Rational& rhs) {
    assert(rhs.m_num != 0 && "Division by zero");

    int64_t g1 = std::gcd(m_num, rhs.m_num);
    int64_t g2 = std::gcd(rhs.m_den, m_den);

    m_num = (m_num / g1) * (rhs.m_den / g2);
    m_den = (m_den / g2) * (rhs.m_num / g1);
    normalize();
    return *this;
}

Rational& Rational::operator^=(const Rational& rhs) {
    assert(rhs.IsInteger() && "Fractional exponent");
    if (rhs.m_num >= 0) {
        m_num = std::pow(m_num, rhs.m_num);
        m_den = std::pow(m_den, rhs.m_num);
    } else {
        m_num = std::pow(m_num, -rhs.m_num);
        m_den = std::pow(m_den, -rhs.m_num);
    }
    return *this;
}

Rational operator+(Rational lhs, const Rational& rhs) {
    lhs += rhs;
    return lhs;
}

Rational operator-(Rational lhs, const Rational& rhs) {
    lhs -= rhs;
    return lhs;
}

Rational operator*(Rational lhs, const Rational& rhs) {
    lhs *= rhs;
    return lhs;
}

Rational operator/(Rational lhs, const Rational& rhs) {
    lhs /= rhs;
    return lhs;
}

Rational operator^(Rational lhs, const Rational& rhs) {
    lhs ^= rhs;
    return lhs;
}

bool Rational::operator==(const Rational& rhs) const {
    return m_num == rhs.m_num && m_den == rhs.m_den;
}

bool Rational::operator!=(const Rational& rhs) const { return !(*this == rhs); }

bool Rational::operator<(const Rational& rhs) const {
    return (__int128_t)m_num * rhs.m_den < (__int128_t)rhs.m_num * m_den;
}

bool Rational::operator>(const Rational& rhs) const {
    return (__int128_t)m_num * rhs.m_den > (__int128_t)rhs.m_num * m_den;
}

bool Rational::operator<=(const Rational& rhs) const { return !(*this > rhs); }

bool Rational::operator>=(const Rational& rhs) const { return !(*this < rhs); }

std::ostream& operator<<(std::ostream& os, const Rational& r) {
    if (r.IsInteger()) {
        os << r.m_num;
    } else {
        os << r.m_num << " / " << r.m_den;
    }
    return os;
}

void Rational::normalize() {
    if (m_den < 0) {
        m_den = -m_den;
        m_num = -m_num;
    }

    int64_t g = std::gcd(m_num, m_den);
    m_num /= g;
    m_den /= g;
}

void Rational::assignNormalized(__int128_t n, __int128_t d) {
    if (d < 0) {
        n = -n;
        d = -d;
    }

    __int128_t a = n < 0 ? -n : n;
    __int128_t b = d;
    while (b != 0) {
        __int128_t temp = b;
        b = a % b;
        a = temp;
    }

    m_num = static_cast<int64_t>(n / a);
    m_den = static_cast<int64_t>(d / a);
}
