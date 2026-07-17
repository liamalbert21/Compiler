#include "Expression.hpp"

Expr::Expr(Literal literal) :
    m_literal{ literal } {}

Literal Expr::getLiteral() const {
    return m_literal;
}

Binary::Binary(Expr left, Token op, Expr right) :
    m_left{ left }, m_op{ op }, m_right{ right } {}

Unary::Unary(Token op, Expr right) :
    m_op{ op }, m_right{ right } {}

Grouping::Grouping(Expr expr) :
    m_expr{ expr } {}