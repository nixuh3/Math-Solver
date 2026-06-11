#include "parser.h"
#include "error.h"

Parser::Parser(const std::vector<Token>& tokens, Arena& arena) : m_tokens(tokens), m_arena(arena) {}

const Expr* Parser::Parse() {
    try {
        return expression();
    } catch (ParseError) {
        return nullptr;
    }
}

const Expr* Parser::expression() {
    const Expr* expr = power();

    while (match(EQUAL)) {
        Token op = previous();
        const Expr* right = power();
        expr = m_arena.Alloc<Expr>(Binary{ expr, op, right });
    }

    return expr;
}

const Expr* Parser::power() {
    const Expr* expr = term();

    while (match(CARET)) {
        Token op = previous();
        const Expr* right = term();
        expr = m_arena.Alloc<Expr>(Binary{ expr, op, right });
    }

    return expr;
}

const Expr* Parser::term() {
    const Expr* expr = factor();

    while (match(MINUS, PLUS)) {
        Token op = previous();
        const Expr* right = factor();
        expr = m_arena.Alloc<Expr>(Binary{ expr, op, right });
    }

    return expr;
}

const Expr* Parser::factor() {
    const Expr* expr = unary();

    while (match(STAR, SLASH)) {
        Token op = previous();
        const Expr* right = unary();
        expr = m_arena.Alloc<Expr>(Binary{ expr, op, right });
    }

    return expr;
}

const Expr* Parser::unary() {
    if (match(PLUS, MINUS)) {
        Token op = previous();
        const Expr* right = unary();
        return m_arena.Alloc<Expr>(Unary{ op, right });
    }

    return primary();
}

const Expr* Parser::primary() {
    if (match(NUMBER)) {
        return m_arena.Alloc<Expr>(Literal{ previous().literal });
    }

    if (match(IDENTIFIER)) {
        return m_arena.Alloc<Expr>(Variable{ previous() });
    }

    if (match(LEFT_PAREN)) {
        const Expr* expr = expression();
        consume(RIGHT_PAREN, "Expected ')' after expression");
        return m_arena.Alloc<Expr>(Grouping{ expr });
    }

    throw error(peek(), "Expected expression");
}

template <typename... Args>
bool Parser::match(Args... args) {
    if (((check(args) ? (advance(), true) : false) || ...)) {
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) {
        return false;
    }
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) {
        m_current++;
    }
    return previous();
}

bool Parser::isAtEnd() { return peek().type == END; }

Token Parser::peek() { return m_tokens[m_current]; }

Token Parser::previous() { return m_tokens[m_current - 1]; }

Token Parser::consume(TokenType type, std::string_view message) {
    if (check(type)) {
        return advance();
    }
    throw error(peek(), message);
}

Parser::ParseError Parser::error(Token token, std::string_view message) {
    if (token.type == TokenType::END) {
        ErrorReporter::CompileError(token.line, " at end", message);
    } else {
        std::string where = " at '" + token.lexeme + "'";
        ErrorReporter::CompileError(token.line, where, message);
    }
    return ParseError();
}
