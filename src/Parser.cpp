#include "Parser.hpp"

#include <cassert>
#include <algorithm>
#include <iostream>

Parser::Parser(std::vector<Token> tokens) :
    m_tokens{ std::move(tokens) } {
        assert(m_tokens.size() > 0);
        m_current = m_tokens.begin();
    }

void Parser::generateAST() {
    m_ast = expression();
}

void Parser::printAST() const {
    m_ast->accept(Expr::Print{});
    std::cout << std::flush;
}

std::optional<Token> Parser::matchTokens(std::initializer_list<Token::Type> types) {    
    Token target{ peek() };
    const auto it{ std::find(types.begin(), types.end(), target.type) };

    if (it == types.end()) {
        return std::nullopt;
    }
    advance();
    return target;
}

std::unique_ptr<Expr> Parser::expression() {
    return term();
}

std::unique_ptr<Expr> Parser::term() {
    std::unique_ptr<Expr> expr{ factor() };

    while (!isEOF()) {
        if (auto op{ matchTokens({ Token::Type::PLUS, Token::Type::MINUS }) }) {
            expr = std::make_unique<Binary>(std::move(expr), op.value(), factor());
        } else {
            break;
        }
    }

    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    std::unique_ptr<Expr> expr{ unary{}.right(*this) };
    
    while (!isEOF()) {
        if (auto op{ matchTokens({ Token::Type::STAR, Token::Type::SLASH }) }) {
            expr = std::make_unique<Binary>(std::move(expr), op.value(), unary{}.right(*this));
        }
        else {
            break;
        }
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary::right(Parser& parser) {
    if (parser.isEOF()) {
        return unary{}.left(parser);
    }

    // Because these operators are right-associative, they requrire that we find the
    // operand AFTER the operator.
    else if (auto op{ parser.matchTokens({ Token::Type::MINUS }) }) {
        return std::make_unique<Unary>(op.value(), unary{}.right(parser));
    }

    return unary{}.left(parser);
}

std::unique_ptr<Expr> Parser::unary::left(Parser& parser) {
    
    // Because these operators are left-associative, they requrire that we find the
    // operand BEFORE the operator.
    std::unique_ptr<Expr> expr{ parser.primary() };

    if (!parser.isEOF()) {
        if (auto op{ parser.matchTokens({ Token::Type::FACTORIAL }) }) {
            expr = std::make_unique<Unary>(op.value(), std::move(expr));
        }
    }

    return expr;
}

std::unique_ptr<Expr> Parser::primary() {
    if (auto num{ matchTokens({ Token::Type::INT, Token::Type::DOUBLE }) }) {
        return std::make_unique<Primary>(num.value());
    }
    else if (auto grouping{ matchTokens({ Token::Type::LEFT_PAREN, Token::Type::LEFT_BRACK }) }) {
        std::unique_ptr<Expr> expr{ std::make_unique<Grouping>(expression()) };

        // The next token MUST correspond to the group type
        Token::Type end{ grouping.value().type == Token::Type::LEFT_PAREN ? Token::Type::RIGHT_PAREN : Token::Type::RIGHT_BRACK };
        assert(matchTokens({ end }) && "ERROR: A group in the expression does not close!");
        
        return expr;
    }
    
    throw std::runtime_error("ERROR: Invalid syntax!");
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
