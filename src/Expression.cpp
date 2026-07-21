#include "Expression.hpp"

Binary::Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) :
    m_left { std::move(left) },
    m_op   { op },
    m_right{ std::move(right) } {}

void Binary::accept(const Visitor& visitor) {
    visitor.visit(*this);
}

Unary::Unary(Token op, std::unique_ptr<Expr> expr) :
    m_op   { op },
    m_expr{ std::move(expr) } {}

void Unary::accept(const Visitor& visitor) {
    visitor.visit(*this);
}

Primary::Primary(Token literal) :
    m_literal{ literal } {}

void Primary::accept(const Visitor& visitor) {
    visitor.visit(*this);
}

Grouping::Grouping(std::unique_ptr<Expr> expr) :
    m_expr{ std::move(expr) } {}

void Grouping::accept(const Visitor& visitor) {
    visitor.visit(*this);
}

void Expr::Eval::visit(Binary& binary)      const { return; }
void Expr::Eval::visit(Unary& binary)       const { return; }
void Expr::Eval::visit(Primary& binary)     const { return; }
void Expr::Eval::visit(Grouping& grouping)  const { return; }

void Expr::Print::visit(Binary& binary)     const { return; }
void Expr::Print::visit(Unary& binary)      const { return; }
void Expr::Print::visit(Primary& binary)    const { return; }
void Expr::Print::visit(Grouping& grouping) const { return; }