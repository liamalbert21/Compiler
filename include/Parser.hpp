#pragma once

#include "Token.hpp"
#include "Expression.hpp"

#include <vector>
#include <optional>
#include <functional>
#include <map>

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    void generateAST();
    void printAST();

private:
    Expr generateExpression(Expr (Parser::* next)(), std::initializer_list<Token::Type> types);
    std::optional<Token> matchTokens(std::initializer_list<Token::Type> types) const;

    Expr term();
    Expr factor();
    Expr unary();
    Expr primary();

    void  advance();
    Token extract();
    void  extract(Token& token);
    Token peek() const;
    void  peek(Token& token) const;
    bool  isEOF() const;

    const std::vector<Token>& m_tokens{};
    std::vector<Token>::const_iterator m_current{};
    Expr m_ast{};
};