#pragma once

#include "token.h"
#include <vector>

class Scanner {
  public:
    explicit Scanner(std::string_view source);
    const std::vector<Token>& Scan();

  private:
    void scanToken();
    void scanNumber();
    void scanIdentifier();

    void addToken(TokenType type);
    void addToken(TokenType type, double literal);

    bool isAtEnd(int offset = 0) const;
    char peek(int offset = 0) const;
    char advance();
    bool match(char expected);

    static bool isDigit(char c);
    static bool isAlpha(char c);
    static bool isAlphaNumeric(char c);

  private:
    std::string_view m_source;
    std::vector<Token> m_tokens;

    int m_start = 0;
    int m_current = 0;
    int m_line = 1;
};
