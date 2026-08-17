#include "Token.hpp"

#include <iostream>

std::string Token::ToString::Type(const Token::Type type) {
    using enum Type;

    switch (type) {
        // Numbers
        case INT:
            return "int";
        case DOUBLE:
            return "double";
        
        // Grouping
        case LEFT_PAREN:
            return "left parenthesis";
        case RIGHT_PAREN:
            return "right parenthesis";
        case LEFT_BRACK:
            return "left bracket";
        case RIGHT_BRACK:
            return "right bracket";

        // Operators
        case PLUS:
            return "sum";
        case MINUS:
            return "difference";
        case STAR:
            return "product";
        case SLASH:
            return "quotient";
        case FACTORIAL:
            return "factorial";

        // These strings should NEVER be displayed to the user
        case DIGIT:
            return "digit";
        case SEPARATOR:
            return "separator";
        case WHITESPACE:
            return "whitespace";

        // Invalid tokens
        case INVALID:
            return "invalid";

        default:
            return "unknown";
    }
}

std::ostream& operator<<(std::ostream& out, const Token& token) {
    switch (token.type) {
        case Token::Type::INT:
            out << std::get<int>(token.literal);
            break;
        case Token::Type::DOUBLE:
            out << std::get<double>(token.literal);
            break;
        default:
            out << std::get<char>(token.literal);
    }

    return out;
}

Token::operator bool() const {
    return type != Type::INVALID;
}

bool Token::operator==(const Token& other) const {
    return type == other.type && literal == other.literal;
}