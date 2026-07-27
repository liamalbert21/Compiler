#pragma once

#include <string>
#include <variant>

using Literal = std::variant<char, int, double>;
using Lexeme  = std::variant<char, std::string>;

struct Token {
    enum class Type {
        INT, DOUBLE,
        LEFT_PAREN, RIGHT_PAREN, LEFT_BRACK, RIGHT_BRACK,
        PLUS, MINUS, STAR, SLASH, FACTORIAL,
        INVALID,

        // Should be invisible to the user
        __DIGIT, __SEPARATOR, __WHITESPACE,
    };

    struct Print {
        static void type(const Token& token);
    };

    explicit operator bool() const;
    bool operator==(const Token& other) const;
    friend std::ostream& operator<<(std::ostream& out, const Token& token);

    Type type{};
    Literal literal{};
    Lexeme lexeme{};
};