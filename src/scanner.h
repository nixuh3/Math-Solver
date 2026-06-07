#pragma once

#include "token.h"
#include <vector>

class Scanner {
  public:
    const std::vector<Token>& Scan(std::string_view source);

  private:
    void ScanToken();
    void ScanNumber();
    void ScanIdentifier();

    void AddToken(TokenType type);
    void AddToken(TokenType type, const Value& literal);

    bool IsAtEnd(int offset = 0) const;
    char Peek(int offset = 0) const;
    char Advance();
    bool Match(char expected);

    static bool IsDigit(char c);
    static bool IsAlpha(char c);
    static bool IsAlphaNumeric(char c);

  private:
    std::string_view m_source;
    std::vector<Token> m_tokens;

    int m_start;
    int m_current;
    int m_line;
};
