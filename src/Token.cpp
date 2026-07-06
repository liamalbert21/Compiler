#include "Token.hpp"

std::string Token::toString(Type type) {
    switch (type) {
        case Type::INT:
            return "int";
        case Type::DOUBLE:
            return "double";
        case Type::LEFT_PAREN:
            return "left parenthesis";
        case Type::RIGHT_PAREN:
            return "right parenthesis";
        case Type::LEFT_BRACK:
            return "left bracket";
        case Type::RIGHT_BRACK:
            return "right bracket";
        case Type::PLUS:
            return "plus sign";
        case Type::MINUS:
            return "minus sign";
        case Type::STAR:
            return "multiplication sign";
        case Type::SLASH:
            return "division sign";

        // These words should never show up. If they do, the lexer failed.
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
    return this->type != Type::INVALID;
}