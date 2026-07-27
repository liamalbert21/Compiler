#pragma once

#include "Expression.hpp"

#include <vector>
#include <optional>

class Parser {
public:
    Parser(std::vector<Token> tokens);
    void generateAST();
    void printAST() const;

private:
    std::optional<Token> matchTokens(std::initializer_list<Token::Type> types);
    std::unique_ptr<Expr> generateExpression(std::initializer_list<Token::Type> types);

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();

    struct unary {
        std::unique_ptr<Expr> right(Parser& parser);
        std::unique_ptr<Expr> left(Parser& parser);
    };

    std::unique_ptr<Expr> primary();

    void  advance();
    Token extract();
    void  extract(Token& token);
    Token peek() const;
    void  peek(Token& token) const;
    bool  isEOF() const;

    std::vector<Token> m_tokens{};
    std::vector<Token>::const_iterator m_current{};
    std::unique_ptr<Expr> m_ast{};
};