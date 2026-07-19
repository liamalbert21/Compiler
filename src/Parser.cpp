#include "Parser.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <algorithm>

Parser::Parser(const std::vector<Token>& tokens) :
    m_tokens{ tokens } {
        assert(m_tokens.size() > 0);
        m_current = m_tokens.begin();
    }

void Parser::generateAST() {
    m_ast = term();
}

void Parser::printAST() const {
    std::cout << "AST printed!" << std::endl;
}

std::optional<Token> Parser::matchTokens(std::initializer_list<Token::Type> types) const {
    Token target{ peek() };
    const auto it{ std::find(types.begin(), types.end(), target.type) };
    return it != types.end() ? std::optional<Token>{ target } : std::nullopt;
}

std::unique_ptr<Expr> Parser::term() {
    std::unique_ptr<Expr> expr{ std::move(factor()) };

    for (bool good_expr{ true }; !isEOF() && good_expr; advance()) {
        if (auto op{ matchTokens({ Token::Type::PLUS, Token::Type::MINUS }) }; op) {
            expr = std::make_unique<Binary>(factor(), op.value(), std::move(expr));
        }
        else {
            good_expr = false;
        }
    }

    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    std::unique_ptr<Expr> expr{ unary() };

    for (bool good_expr{ true }; !isEOF() && good_expr; advance()) {
        if (auto op{ matchTokens({ Token::Type::STAR, Token::Type::SLASH }) }; op) {
            expr = std::make_unique<Binary>(unary(), op.value(), std::move(expr));
        }
        else {
            good_expr = false;
        }
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary() {
    std::unique_ptr<Expr> expr{ primary() };

    for (bool good_expr{ true }; !isEOF() && good_expr; advance()) {
        if (auto op{ matchTokens({ Token::Type::MINUS }) }; op) {
            expr = std::make_unique<Binary>(primary(), op.value(), std::move(expr));
        }
        else {
            good_expr = false;
        }
    }

    return expr;
}

std::unique_ptr<Expr> Parser::primary() {
    if (matchTokens({ Token::Type::INT, Token::Type::DOUBLE })) {
        return std::make_unique<Primary>(extract());
    }
    else if (matchTokens({ Token::Type::LEFT_PAREN, Token::Type::LEFT_BRACK })) {
        // Find corresponding right-side token. Throw an exception if not
        return {};
    }
    throw std::runtime_error("ERROR: Terminal is not a number or grouping symbol!");
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
