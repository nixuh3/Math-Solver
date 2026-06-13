#pragma once

#include "token.h"
#include <variant>

struct Expr;

struct Binary {
    const Expr* left;
    const Token op;
    const Expr* right;
};

struct Unary {
    const Token op;
    const Expr* right;
};

struct Grouping {
    const Expr* expression;
};

struct Literal {
    double value;
};

struct Variable {
    std::string_view name;
};

struct Expr : std::variant<Binary, Grouping, Literal, Unary, Variable> {
    using variant::variant;
};

struct Equation {
    const Expr* left;
    const Expr* right;
};
