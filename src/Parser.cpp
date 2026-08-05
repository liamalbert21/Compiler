#include "Parser.hpp"
#include "Log.hpp"

#include <cassert>
#include <algorithm>
#include <iostream>
#include <sstream>

using TokenTypes = std::initializer_list<Token::Type>;

std::string Error<Parser>::ExpectedExpression(const Parser& parser, Token::Type type, Expr::OperandSide side) {
    std::ostringstream message{};
    message << "\nERROR: Expected expression "
            << (
                    side == Expr::OperandSide::LEFT  ? "before " : 
                    side == Expr::OperandSide::RIGHT ? "after "  : "before/after "
                )
            << Token::ToString::Type(type) << "!\n";

    // Need -2 because of the newlines
    std::size_t width{ message.str().length() - 2 };
    message << std::string(width, '-') << '\n' << "* At token instance: "
            << std::count_if(
                parser.m_tokens.begin(),
                parser.m_current,
                [=](const Token& token) { return token.type == type; }
                )
            << '\n' << std::string(width, '-');

    return message.str();
}

Parser::Parser(std::vector<Token> tokens) :
    m_state{ State::INIT }, m_tokens{ std::move(tokens) } {
        if (m_tokens.size()) {
            m_current = m_tokens.begin();
        }
    }

bool Parser::generateAST() {
    // Assume "expression" will produce a valid AST
    m_state = State::OKAY;
    m_ast = expression();
    
    // Should never throw
    if (!isEOF()) {
        throw std::runtime_error("CODE FAULT: A complete AST was produced without examining all tokens");
    }

    return m_state == State::OKAY;
}

void Parser::printAST() const {
    if (m_state != State::OKAY) {
        return;
    }

    std::cout << '\n';
    m_ast->accept(Expr::Print{});
    std::cout << std::flush;
}

std::optional<Token> Parser::matchTokens(TokenTypes types) {    
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

void Parser::handleMissingOperand(Token::Type op, Expr::OperandSide side) {
    Log::instance().error(Error<Parser>::ExpectedExpression(*this, op, side));
    m_state = State::FAIL;
}

Expr::OperandSide Parser::getMissingOperandSide(std::unique_ptr<Expr>& left, std::unique_ptr<Expr>& right) {
    return !left &&  right ? Expr::OperandSide::LEFT  :
            left && !right ? Expr::OperandSide::RIGHT :
                             Expr::OperandSide::UNKNOWN;
}

std::unique_ptr<Expr> Parser::expression() {
    return term();
}

std::unique_ptr<Expr> Parser::term() {
    static constexpr TokenTypes types{ Token::Type::PLUS, Token::Type::MINUS };
    
    std::unique_ptr<Expr> expr{ factor() };

    // Initializes op on every iteration
    while (auto op{ matchTokens(types) }) {
        std::unique_ptr<Expr> right{ factor() };

        if (!expr || !right) {
            handleMissingOperand(op.value().type, getMissingOperandSide(expr, right));
        }

        expr = std::make_unique<Binary>(std::move(expr), op.value(), std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    static constexpr TokenTypes types{ Token::Type::STAR, Token::Type::SLASH };

    std::unique_ptr<Expr> expr{ unary::right(*this) };
    
    while (auto op{ matchTokens(types) }) {
        std::unique_ptr<Expr> right{ unary::right(*this) };

        if (!expr || !right) {
            handleMissingOperand(op.value().type, getMissingOperandSide(expr, right));
        }

        expr = std::make_unique<Binary>(std::move(expr), op.value(), std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary::right(Parser& parser) {
    static constexpr TokenTypes types{ Token::Type::MINUS };

    // Because these operators are right-associative, they requrire that we find
    // the operand AFTER the operator.
    if (auto op{ parser.matchTokens(types) }) {
        std::unique_ptr<Expr> expr{ unary::left(parser) };
        if (!expr) {
            parser.handleMissingOperand(op.value().type, Expr::OperandSide::RIGHT);
        }

        return std::make_unique<Unary>(op.value(), std::move(expr));
    }

    return unary::left(parser);
}

std::unique_ptr<Expr> Parser::unary::left(Parser& parser) {
    static constexpr TokenTypes types{ Token::Type::FACTORIAL };
    
    // Because these operators are left-associative, they require that we find 
    // the operand BEFORE the operator.
    std::unique_ptr<Expr> expr{ parser.primary() };

    if (auto op{ parser.matchTokens(types) }) {
        if (!expr) {
            parser.handleMissingOperand(op.value().type, Expr::OperandSide::LEFT);
        }

        expr = std::make_unique<Unary>(op.value(), std::move(expr));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::primary() {
    static constexpr TokenTypes numeric_types{ Token::Type::INT, Token::Type::DOUBLE };
    static constexpr TokenTypes grouping_types{ Token::Type::LEFT_PAREN, Token::Type::LEFT_BRACK };

    std::unique_ptr<Expr> expr{ nullptr };

    if (auto num{ matchTokens(numeric_types) }) {
        expr = std::make_unique<Primary>(num.value());
    }
    else if (auto grouping{ matchTokens(grouping_types) }) {
        expr = std::make_unique<Grouping>(expression());

        // The next token MUST correspond to the group type
        Token::Type expected{
            grouping.value().type == Token::Type::LEFT_PAREN ? Token::Type::RIGHT_PAREN : Token::Type::RIGHT_BRACK
        };

        assert(matchTokens({ expected }) && "Critical error: A group in the expression does not close!");
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