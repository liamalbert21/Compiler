#include "Parser.hpp"
#include "Log.hpp"

#include <cassert>
#include <algorithm>
#include <sstream>

using TokenTypes = std::initializer_list<Token::Type>;

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
    
    if (!isEOF()) {
        Error<Parser>::ExpectedOperand(*this);
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
            Error<Parser>::ExpectedExpression(
                *this,
                getMissingOperandSide(expr, right),
                op.value().type
            );
        }

        expr = std::make_unique<Binary>(std::move(expr), op.value(), std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    static constexpr TokenTypes types{ Token::Type::STAR, Token::Type::SLASH };

    std::unique_ptr<Expr> expr{ unary::right(*this) };
    
    while (auto op{ matchTokens(types) }) {
        // Not assigning an alias to op.value() 
        
        std::unique_ptr<Expr> right{ unary::right(*this) };

        if (!expr || !right) {
            Error<Parser>::ExpectedExpression(
                *this,
                getMissingOperandSide(expr, right),
                op.value().type
            );
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
            Error<Parser>::ExpectedExpression(
                parser,
                Expr::OperandSide::RIGHT,
                op.value().type
            );
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
            Error<Parser>::ExpectedExpression(
                parser,
                Expr::OperandSide::LEFT,
                op.value().type
            );
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

void Parser::ErrorWrapper(
    std::string_view start,
    std::function<std::string(std::pair<Content, std::size_t>)> func
) {
    std::ostringstream message{};
    const std::size_t width{ start.length() };

    message << '\n' << start << '\n'
            << std::string(width, '-') << '\n'
            << func({ m_tokens, static_cast<std::size_t>(m_current - m_tokens.begin()) })
            << std::string(width, '-');

    Log::instance().error(message.str());
    m_state = State::FAIL;
}

void Error<Parser>::ExpectedExpression(Parser& parser, Expr::OperandSide side, Token::Type type) {
    auto description{ [=](Context data) -> std::string {
        std::ostringstream oss{};
        auto start{ std::get<std::vector<Token>>(data.first).begin() };

        oss << "* At token instance: "
            << std::count_if(
                    start,
                    start + data.second,
                    [=](const Token& token) { return token.type == type; }
                )
            << '\n';

        return oss.str();
    }};

    std::ostringstream message{};
    message << "Expected expression "
            << (
                    side == Expr::OperandSide::LEFT  ? "before " : 
                    side == Expr::OperandSide::RIGHT ? "after "  : "before/after "
                )
            << Token::ToString::Type(type) << '!';

    parser.ErrorWrapper(message.str(), std::function<std::string(Context)>{ description });
}

void Error<Parser>::ExpectedOperand(Parser& parser) {
    auto description{ [](Context data) -> std::string {
        std::ostringstream oss{};
        oss << "Location: Token instance " << data.second + 1 << '\n';
        return oss.str();
    }};

    std::string message{ "Expected operand!" };
    parser.ErrorWrapper(message, std::function<std::string(Context)>{ description });
}