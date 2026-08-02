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
            return "plus sign";
        case MINUS:
            return "minus sign";
        case STAR:
            return "multiplication sign";
        case SLASH:
            return "division sign";
        case FACTORIAL:
            return "factorial sign";

        // These strings should never be displayed to the user
        case __DIGIT:
            return "digit";
        case __SEPARATOR:
            return "separator";
        case __WHITESPACE:
            return "whitespace";

        // Invalid tokens
        case INVALID:
            return "invalid";

        default:
            return "unknown";
    }
}

/**
 * @brief Prints the provided token's literal
 *
 * @param out             output stream (LHS)
 * @param token           token input   (RHS)
 * @return std::ostream&  reference to LHS    
 */
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
    return type == other.type && literal == other.literal && lexeme == other.lexeme;
}