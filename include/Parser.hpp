#pragma once

#include "Error.hpp"
#include "Expression.hpp"

#include <vector>
#include <optional>

class Parser : private Pipeline {
    friend struct Error<Parser>;

public:
    Parser(std::vector<Token> tokens);

    bool generateAST();
    void printAST() const;

private:
    std::optional<Token> matchTokens(std::initializer_list<Token::Type> types);
    void handleMissingOperand(Token::Type op, Expr::OperandSide side);
    Expr::OperandSide getMissingOperandSide(std::unique_ptr<Expr>& left, std::unique_ptr<Expr>& right);

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();

    struct unary {
        static std::unique_ptr<Expr> right(Parser& parser);
        static std::unique_ptr<Expr> left(Parser& parser);
    };

    std::unique_ptr<Expr> primary();

    void  advance();
    Token extract();
    void  extract(Token& token);
    Token peek() const;
    void  peek(Token& token) const;
    bool  isEOF() const;

    State m_state{};
    std::vector<Token> m_tokens{};
    std::vector<Token>::const_iterator m_current{};
    std::unique_ptr<Expr> m_ast{};
};

template <>
struct Error<Parser> {
    static std::string ExpectedExpression(const Parser& parser, Token::Type type, Expr::OperandSide side);
};