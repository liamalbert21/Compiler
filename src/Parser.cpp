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
    
    if (!isEOF()) {
        // Eventually, be more descriptive. Say something like: "ERROR: A group is closed when it never began!"
        throw std::runtime_error("ERROR: Invalid syntax!");
    }
}

void Parser::printAST() const {
    std::cout << '\n';
    m_ast->accept(Expr::Print{});
    std::cout << std::flush;
}

std::optional<Token> Parser::matchTokens(std::initializer_list<Token::Type> types) {    
    if (isEOF()) {
        return std::nullopt;
    }
    
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
    static constexpr std::initializer_list<Token::Type> types{ Token::Type::PLUS, Token::Type::MINUS };
    std::unique_ptr<Expr> expr{ factor() };
    auto op{ matchTokens(types) };

    while (op) {
        expr = std::make_unique<Binary>(std::move(expr), op.value(), factor());
        op = matchTokens(types);
    }

    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    static constexpr std::initializer_list<Token::Type> types{ Token::Type::STAR, Token::Type::SLASH };
    std::unique_ptr<Expr> expr{ unary{}.right(*this) };
    auto op{ matchTokens(types) };

    while (op) {
        expr = std::make_unique<Binary>(std::move(expr), op.value(), unary{}.right(*this));
        op = matchTokens(types);
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary::right(Parser& parser) {
    static constexpr std::initializer_list<Token::Type> types{ Token::Type::MINUS };

    // Because these operators are right-associative, they requrire that we find the
    // operand AFTER the operator.
    if (auto op{ parser.matchTokens(types) }) {
        return std::make_unique<Unary>(op.value(), unary{}.right(parser));
    }

    return unary{}.left(parser);
}

std::unique_ptr<Expr> Parser::unary::left(Parser& parser) {
    static constexpr std::initializer_list<Token::Type> types{ Token::Type::FACTORIAL };
    
    // Because these operators are left-associative, they requrire that we find the
    // operand BEFORE the operator.
    std::unique_ptr<Expr> expr{ parser.primary() };

    if (auto op{ parser.matchTokens(types) }) {
        expr = std::make_unique<Unary>(op.value(), std::move(expr));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::primary() {
    std::unique_ptr<Expr> expr{ nullptr };

    if (auto num{ matchTokens({ Token::Type::INT, Token::Type::DOUBLE }) }) {
        expr = std::make_unique<Primary>(num.value());
    }
    else if (auto grouping{ matchTokens({ Token::Type::LEFT_PAREN, Token::Type::LEFT_BRACK }) }) {
        expr = std::make_unique<Grouping>(expression());

        // The next token MUST correspond to the group type
        Token::Type expected{ grouping.value().type == Token::Type::LEFT_PAREN ? Token::Type::RIGHT_PAREN : Token::Type::RIGHT_BRACK };
        assert(matchTokens({ expected }) && "ERROR: A group in the expression does not close!");
    }

    return expr;
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
