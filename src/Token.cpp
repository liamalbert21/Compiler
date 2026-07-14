#include "Token.hpp"

// This function should be automatically generated
std::string Token::toString(Type type) {
    switch (type) {
        // Numbers
        case Type::INT:
            return "int";
        case Type::DOUBLE:
            return "double";
        
        // Grouping
        case Type::LEFT_PAREN:
            return "left parenthesis";
        case Type::RIGHT_PAREN:
            return "right parenthesis";
        case Type::LEFT_BRACK:
            return "left bracket";
        case Type::RIGHT_BRACK:
            return "right bracket";

        // Operators
        case Type::PLUS:
            return "plus sign";
        case Type::MINUS:
            return "minus sign";
        case Type::STAR:
            return "multiplication sign";
        case Type::SLASH:
            return "division sign";
        case Type::FACTORIAL:
            return "factorial sign";

        // These strings should never be displayed to the user
        case Type::__DIGIT:
            return "digit";
        case Type::__SEPARATOR:
            return "separator";
        case Type::__WHITESPACE:
            return "whitespace";
    }
    
    return "unknown";
}

Token::operator bool() const {
    return type != Type::INVALID;
}