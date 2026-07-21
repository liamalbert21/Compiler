#include "Parser.hpp"

#include <cassert>
#include <stdexcept>
#include <algorithm>

Parser::Parser(const std::vector<Token>& tokens) :
    m_tokens{ tokens } {
        assert(m_tokens.size() > 0);
        m_current = m_tokens.begin();
    }

void Parser::generateAST() {
    m_ast = expression();
}

void Parser::printAST() const {
    m_ast->accept(Expr::Print{});
}

std::optional<Token> Parser::matchTokens(std::initializer_list<Token::Type> types) const {
    // Must check this before attempting to peek at the target (current) token
    if (isEOF()) {
        return std::nullopt;
    }
    
    Token target{ peek() };
    const auto it{ std::find(types.begin(), types.end(), target.type) };
    return it != types.end() ? std::optional<Token>{ target } : std::nullopt;
}

std::unique_ptr<Expr> Parser::expression() {
    return term();
}

std::unique_ptr<Expr> Parser::term() {
    std::unique_ptr<Expr> expr{ factor() };

    for (auto op{ matchTokens({ Token::Type::PLUS, Token::Type::MINUS }) }; op;) {
        advance();
        expr = std::make_unique<Binary>(std::move(expr), op.value(), factor());
    }

    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    std::unique_ptr<Expr> expr{ unary() };

    for (auto op{ matchTokens({ Token::Type::STAR, Token::Type::SLASH }) }; op;) {
        advance();
        expr = std::make_unique<Binary>(std::move(expr), op.value(), unary());
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary() {
    std::unique_ptr<Expr> expr{ primary() };

    for (auto op{ matchTokens({ Token::Type::MINUS, Token::Type::FACTORIAL }) }; op;) {
        advance();

        /**
         * If expr == nullptr, the operand is the next token (minus)
         * Otherwise, it's the already-extracted token (factorial)
         */
        expr = std::make_unique<Unary>(op.value(), expr ? std::move(expr) : unary());
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::primary() {
    std::unique_ptr<Expr> expr{ nullptr };

    if (auto num{ matchTokens({ Token::Type::INT, Token::Type::DOUBLE }) }; num) {
        expr = std::make_unique<Primary>(num.value());
    }
    else if (auto grouping{ matchTokens({ Token::Type::LEFT_PAREN, Token::Type::LEFT_BRACK }) }; grouping) {
        expr = std::make_unique<Grouping>(expression());

        // The next token MUST correspond to the group type
        Token::Type end{ grouping.value().type == Token::Type::LEFT_PAREN ? Token::Type::RIGHT_PAREN : Token::Type::RIGHT_BRACK };
        assert(matchTokens({ end }) && "ERROR: A group in the expression does not close!");
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
