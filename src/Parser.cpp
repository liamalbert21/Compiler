#include "Parser.hpp"

#include <iostream>
#include <cassert>
#include <stdexcept>

/**
 * \todo
 * -----------------------
 * 1) implement a custom hash for the Generator -> Expr::Type map
 * 3) add grouping functionality in primary()
 * 4) error handling (across the lexer, too)
 * 
 * Then push and continue with compiling into bytecode. This will take a few days as it's quite complex.
 */

Parser::Parser(const std::vector<Token>& tokens) :
    m_tokens{ tokens } {
        assert(m_tokens.size() > 0);
        m_current = m_tokens.begin();
    }

void Parser::generateAST() {
    m_ast = term();
}

void Parser::printAST() {
    std::cout << "AST Printed!" << std::endl;
}

Expr Parser::generateExpression(Expr (Parser::* next)(), std::initializer_list<Token::Type> types) {
    Expr expr{ next() };
    // auto type{ getPrecedenceHash()[next] };
    Expr::Type type = Expr::Type::BINARY;

    for (auto op{ matchTokens(types) }; op && !isEOF(); advance()) {
        switch (type) {
            case Expr::Type::BINARY:
                expr = Binary{ expr, op.value(), next() };
                break;
            case Expr::Type::UNARY:
                expr = Unary{ op.value(), expr };
                break;
        }
    }

    return expr;
}

Expr Parser::term() {    
    static Expr (Parser::* next)(){ factor };
    return generateExpression(next, { Token::Type::PLUS, Token::Type::MINUS });
}

Expr Parser::factor() {
    static Expr (Parser::* next)(){ unary };
    return generateExpression(next, { Token::Type::STAR, Token::Type::SLASH });
}

Expr Parser::unary() {
    static Expr (Parser::* next)(){ primary };
    return generateExpression(next, { Token::Type::MINUS });
}

Expr Parser::primary() {
    if (matchTokens({ Token::Type::INT, Token::Type::DOUBLE })) {
        return { extract().literal };
    }
    else if (matchTokens({ Token::Type::LEFT_PAREN, Token::Type::LEFT_BRACK })) {
        // Find corresponding right-side token. Throw an exception if not
        return {};
    }
    throw std::runtime_error("ERROR: Terminal is not a number or grouping symbol!");
}

std::optional<Token> Parser::matchTokens(std::initializer_list<Token::Type> types) const {
    Token target{ peek() };
    for (Token::Type type : types) {
        if (type == target.type) {
            return target;
        }
    }
    return std::nullopt;
}

void Parser::advance() {
    ++m_current;
}

Token Parser::extract() {
    return *(m_current++);
}

void Parser::extract(Token& token) {
    token = *(m_current++);
}

Token Parser::peek() const {
    return *m_current;
}

void Parser::peek(Token& token) const {
    token = *m_current;
}

bool Parser::isEOF() const {
    return m_current == m_tokens.end();
}