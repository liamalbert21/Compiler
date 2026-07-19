#pragma once

#include "Expression.hpp"

#include <vector>
#include <optional>
#include <map>

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    void generateAST();
    void printAST() const;

private:
    std::optional<Token> matchTokens(std::initializer_list<Token::Type> types) const;

    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

    void  advance();
    Token extract();
    void  extract(Token& token);
    Token peek() const;
    void  peek(Token& token) const;
    bool  isEOF() const;

    const std::vector<Token>& m_tokens{};
    std::vector<Token>::const_iterator m_current{};
    std::unique_ptr<Expr> m_ast{};
};