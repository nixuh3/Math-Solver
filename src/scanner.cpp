#include "scanner.h"
#include "error.h"

const std::vector<Token>& Scanner::Scan(std::string_view source) {
    m_source = source;
    m_tokens.clear();
    m_start = 0;
    m_current = 0;
    m_line = 1;

    while (!IsAtEnd()) {
        m_start = m_current;
        ScanToken();
    }

    m_tokens.push_back({ END, "", {}, m_line });
    return m_tokens;
}

void Scanner::ScanToken() {
    char c = Advance();

    switch (c) {
        case '(': AddToken(LEFT_PAREN); break;
        case ')': AddToken(RIGHT_PAREN); break;
        case '.': AddToken(DOT); break;
        case '-': AddToken(MINUS); break;
        case '+': AddToken(PLUS); break;
        case '*': AddToken(STAR); break;
        case '/': AddToken(SLASH); break;
        case '=': AddToken(EQUAL); break;
        case ' ':
        case '\r':
        case '\t': break;
        case '\n': m_line++; break;
        default:
            if (IsDigit(c)) {
                ScanNumber();
            } else if (IsAlpha(c)) {
                ScanIdentifier();
            } else {
                ErrorReporter::CompileError(
                    m_line, std::string("Unexpected character '") + c + "'");
            }
            break;
    }
}

void Scanner::ScanNumber() {
    while (IsDigit(Peek())) {
        Advance();
    }

    if (Peek() == '.' && IsDigit(Peek(1))) {
        Advance();

        while (IsDigit(Peek())) {
            Advance();
        }
    }

    std::string s(m_source.begin() + m_start, m_source.begin() + m_current);
    AddToken(NUMBER, std::stod(s));
}

void Scanner::ScanIdentifier() {
    while (IsAlphaNumeric(Peek())) {
        Advance();
    }

    std::string text(m_source.begin() + m_start, m_source.begin() + m_current);
    AddToken(IDENTIFIER);
}

void Scanner::AddToken(TokenType type) { AddToken(type, {}); }

void Scanner::AddToken(TokenType type, const Value& literal) {
    std::string_view text(&m_source[m_start], m_current - m_start);
    m_tokens.emplace_back(type, text, literal, m_line);
}

bool Scanner::IsAtEnd(int offset) const { return m_current + offset >= m_source.length(); }
char Scanner::Peek(int offset) const {
    if (IsAtEnd(offset)) {
        return '\0';
    }
    return m_source[m_current + offset];
}

char Scanner::Advance() { return m_source[m_current++]; }

bool Scanner::Match(char expected) {
    if (IsAtEnd() || m_source[m_current] != expected) {
        return false;
    }

    m_current++;
    return true;
}

bool Scanner::IsDigit(char c) { return c >= '0' && c <= '9'; }

bool Scanner::IsAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::IsAlphaNumeric(char c) { return IsAlpha(c) || IsDigit(c); }
