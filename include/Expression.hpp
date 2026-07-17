#pragma once

#include "Token.hpp"

class Expr {
public:
    enum class Type {
        BINARY, UNARY, PRIMARY
    };

    Expr() = default;
    Expr(Literal literal);
    Literal getLiteral() const;

private:
    Literal m_literal{};
};

class Binary : public Expr {
public:
    Binary(Expr left, Token op, Expr right);

private:
    Expr m_left{}, m_right{};
    Token m_op{};
};

class Unary : public Expr {
public:
    Unary(Token op, Expr right);

private:
    Expr m_right{};
    Token m_op{};
};

class Grouping : public Expr {
public:
    Grouping(Expr expr);

private:
    Expr m_expr{};
};