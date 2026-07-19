#include "Expression.hpp"

Binary::Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) :
    m_left { std::move(left) },
    m_op   { op },
    m_right{ std::move(right) } {}

void Binary::accept(Visitor& visitor) {
    visitor.visit(*this);
}

Unary::Unary(Token op, std::unique_ptr<Expr> right) :
    m_op   { op },
    m_right{ std::move(right) } {}

void Unary::accept(Visitor& visitor) {
    visitor.visit(*this);
}

Primary::Primary(Token literal) :
    m_literal{ literal } {}

void Primary::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Expr::Eval::visit(Binary& binary)  { return; }
void Expr::Eval::visit(Unary& binary)   { return; }
void Expr::Eval::visit(Primary& binary) { return; }

void Expr::Print::visit(Binary& binary)  { return; }
void Expr::Print::visit(Unary& binary)   { return; }
void Expr::Print::visit(Primary& binary) { return; }