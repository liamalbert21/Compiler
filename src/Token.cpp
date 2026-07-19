#include "Token.hpp"

std::string Token::toString(Type type) {
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
    }
    
    return "unknown";
}

Token::operator bool() const {
    return type != Type::INVALID;
}

bool Token::operator==(const Token& other) const {
    return type == other.type && literal == other.literal && lexeme == other.lexeme;
}