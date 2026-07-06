#pragma once

#include <string>
#include <variant>

using Literal = std::variant<char, int, double>;
using Lexeme  = std::variant<char, std::string>;

struct Token {
    enum class Type {
        INT, DOUBLE,
        LEFT_PAREN, RIGHT_PAREN, LEFT_BRACK, RIGHT_BRACK,
        PLUS, MINUS, STAR, SLASH,
        INVALID,

        // Should be invisible to the user
        __DIGIT, __SEPARATOR, __WHITESPACE,
    };

    static std::string toString(Type type);
    operator bool() const;

    Type type{};
    Literal literal{};
    Lexeme lexeme{};
};