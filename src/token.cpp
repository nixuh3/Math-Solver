#include "token.h"
#include <unordered_map>
#include <iomanip>
#include <cmath>
#include <sstream>

namespace {

std::string FormatDouble(double x, int precision = 6) {
    if (std::isnan(x)) {
        return "nan";
    } else if (std::isinf(x)) {
        return x > 0 ? "inf" : "-inf";
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << x;

    std::string s = oss.str();

    auto pos = s.find_last_not_of('0');
    if (pos != std::string::npos) {
        s.erase(pos + 1);
    }

    if (!s.empty() && s.back() == '.') {
        s.pop_back();
    }

    return s;
}

const std::unordered_map<TokenType, std::string_view> TypeToStr{
    {  LEFT_PAREN,  "left_paren" },
    { RIGHT_PAREN, "right_paren" },
    {         DOT,         "dot" },
    {       MINUS,       "minus" },
    {        PLUS,        "plus" },
    {       SLASH,       "slash" },
    {        STAR,        "star" },
    {       EQUAL,       "equal" },
    {  IDENTIFIER,  "identifier" },
    {      NUMBER,      "number" },
    {         END,         "end" }
};

} // namespace

std::string ValToStr(const Value& value) {
    return std::visit(
        [](const auto& val) -> std::string {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, std::string>) {
                return val;
            } else if constexpr (std::is_same_v<T, double>) {
                return FormatDouble(val);
            } else {
                return "";
            }
        },
        value);
}

Token::Token(TokenType type, std::string_view lexeme, const Value& value, int line)
    : type(type), lexeme(lexeme), literal(value), line(line) {}

std::string Token::ToStr() const {
    return std::string(TypeToStr.at(type)) + " " + lexeme + " " + ValToStr(literal);
}
