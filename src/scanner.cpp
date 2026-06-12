#include "pch.h"
#include "scanner.h"
#include "error.h"

Scanner::Scanner(std::string_view source) : m_source(source) {}

const std::vector<Token>& Scanner::Scan() {
    while (!isAtEnd()) {
        m_start = m_current;
        scanToken();
    }

    m_tokens.push_back({ END, "", {}, m_line });
    return m_tokens;
}

void Scanner::scanToken() {
    char c = advance();

    switch (c) {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '+': addToken(PLUS); break;
        case '-': addToken(MINUS); break;
        case '*': addToken(STAR); break;
        case '/': addToken(SLASH); break;
        case '^': addToken(CARET); break;
        case '=': addToken(EQUAL); break;
        case ' ':
        case '\r':
        case '\t': break;
        case '\n': m_line++; break;
        default:
            if (isDigit(c)) {
                scanNumber();
            } else if (isAlpha(c)) {
                scanIdentifier();
            } else {
                ErrorReporter::CompileError(
                    m_line, std::string("Unexpected character '") + c + "'");
            }
            break;
    }
}

void Scanner::scanNumber() {
    while (isDigit(peek())) {
        advance();
    }

    if (peek() == '.' && isDigit(peek(1))) {
        advance();

        while (isDigit(peek())) {
            advance();
        }
    }

    std::string s(m_source.begin() + m_start, m_source.begin() + m_current);
    addToken(NUMBER, std::stod(s));
}

void Scanner::scanIdentifier() {
    while (isAlphaNumeric(peek())) {
        advance();
    }

    std::string text(m_source.begin() + m_start, m_source.begin() + m_current);
    addToken(IDENTIFIER);
}

void Scanner::addToken(TokenType type) { addToken(type, {}); }

void Scanner::addToken(TokenType type, double literal) {
    std::string_view text(&m_source[m_start], m_current - m_start);
    m_tokens.emplace_back(type, text, literal, m_line);
}

bool Scanner::isAtEnd(int offset) const { return m_current + offset >= m_source.length(); }

char Scanner::peek(int offset) const {
    if (isAtEnd(offset)) {
        return '\0';
    }
    return m_source[m_current + offset];
}

char Scanner::advance() { return m_source[m_current++]; }

bool Scanner::match(char expected) {
    if (isAtEnd() || m_source[m_current] != expected) {
        return false;
    }

    m_current++;
    return true;
}

bool Scanner::isDigit(char c) { return c >= '0' && c <= '9'; }

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }
