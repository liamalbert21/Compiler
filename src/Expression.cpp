#include "Expression.hpp"

Binary::Binary(Expr left, Token op, Expr right) :
    m_left{left}, m_op{op}, m_right{right} {}

Unary::Unary(Token op, Expr right) :
    m_op{op}, m_right{right} {}

Grouping::Grouping(Expr expr) :
    m_expr{expr} {}