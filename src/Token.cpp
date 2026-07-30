#include "Token.hpp"

#include <iostream>

void Token::Print::type(const Token& token) {
    using enum Type;

    switch (token.type) {
        // Numbers
        case INT:
            std::cout << "int";
            break;
        case DOUBLE:
            std::cout << "double";
            break;
        
        // Grouping
        case LEFT_PAREN:
            std::cout << "left parenthesis";
            break;
        case RIGHT_PAREN:
            std::cout << "right parenthesis";
            break;
        case LEFT_BRACK:
            std::cout << "left bracket";
            break;
        case RIGHT_BRACK:
            std::cout << "right bracket";
            break;

        // Operators
        case PLUS:
            std::cout << "plus sign";
            break;
        case MINUS:
            std::cout << "minus sign";
            break;
        case STAR:
            std::cout << "multiplication sign";
            break;
        case SLASH:
            std::cout << "division sign";
            break;
        case FACTORIAL:
            std::cout << "factorial sign";
            break;

        // These strings should never be displayed to the user
        case __DIGIT:
            std::cout << "digit";
            break;
        case __SEPARATOR:
            std::cout << "separator";
            break;
        case __WHITESPACE:
            std::cout << "whitespace";
            break;

        // Invalid tokens
        case INVALID:
            std::cout << "invalid";
            break;

        default:
            std::cout << "unknown";
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