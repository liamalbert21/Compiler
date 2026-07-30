#include "Expression.hpp"

#include <iostream>

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

Primary::Primary(const Token& token) :
    m_value{ token } {}

void Primary::accept(const Visitor& visitor) {
    visitor.visit(*this);
}

Grouping::Grouping(std::unique_ptr<Expr> expr) :
    m_expr{ std::move(expr) } {}

void Grouping::accept(const Visitor& visitor) {
    visitor.visit(*this);
}

void Expr::Eval::visit(Binary& binary) const {
    return;
}

void Expr::Eval::visit(Unary& unary) const {
    return;
}

void Expr::Eval::visit(Primary& primary) const {
    return;
}

void Expr::Eval::visit(Grouping& grouping) const {
    return;
}

void Expr::Print::visit(Binary& binary) const {
    std::cout << binary.m_op << ' ';
    binary.m_left->accept(Expr::Print{});
    binary.m_right->accept(Expr::Print{});
}

void Expr::Print::visit(Unary& unary) const {
    std::cout << unary.m_op << ' ';
    unary.m_expr->accept(Expr::Print{});
}

void Expr::Print::visit(Primary& primary) const {
    std::cout << primary.m_value << ' ';
}

void Expr::Print::visit(Grouping& grouping) const {
    std::cout << "{ ";
    grouping.m_expr->accept(Expr::Print{});
    std::cout << "} ";
}