#pragma once

#include <string>
#include <variant>

using Literal = std::variant<char, int, double>;

struct Token {
    enum class Type {
        INT, DOUBLE,
        LEFT_PAREN, RIGHT_PAREN, LEFT_BRACK, RIGHT_BRACK,
        PLUS, MINUS, STAR, SLASH, FACTORIAL,
        INVALID,

        // Should be invisible to the user
        DIGIT, SEPARATOR, WHITESPACE,
    };

    struct ToString {
        static std::string Type(const Token::Type type);
    };

    explicit operator bool() const;
    bool operator==(const Token& other) const;

    /**
     * @brief Prints the provided token's literal
     *
     * @param out             output stream (LHS)
     * @param token           token input   (RHS)
     * @return std::ostream&  reference to LHS    
     */
    friend std::ostream& operator<<(std::ostream& out, const Token& token);

    Type type{};
    Literal literal{};
};