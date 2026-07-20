#pragma once

#include "Token.hpp"

#include <memory>

class Binary; class Unary; class Primary; class Grouping;

/**
 * Because every instance that relates to Expr must be a pointer (as Expr
 * is abstract), children must inherit Expr publicly to be able to match the
 * Expr type when recursively assigning child nodes that need to be interpreted
 * as pointers to Expr.
 */
class Expr {
public:
    class Visitor {
    public:
        virtual void visit(Binary& binary)     = 0;
        virtual void visit(Unary& unary)       = 0;
        virtual void visit(Primary& primary)   = 0;
        virtual void visit(Grouping& grouping) = 0;
    };

    class Eval : public Visitor {
    public:
        void visit(Binary& binary) override;
        void visit(Unary& unary) override;
        void visit(Primary& primary) override;
        void visit(Grouping& grouping) override;
    };

    class Print : public Visitor {
    public:
        void visit(Binary& binary) override;
        void visit(Unary& unary) override;
        void visit(Primary& primary) override;
        void visit(Grouping& grouping) override;
    };

    /**
     * Deleting an object through a pointer to base invokes undefined behavior unless
     * the destructor in the base class is virtual. Otherwise, calling code can attempt to destroy
     * a derived class object through a base class pointer.
     * 
     * ----- e.g. -----
     * std::unique_ptr<Expr> expr{ std::make_unique<Binary>(...) };
     * ~expr(); // 'expr' is a base class pointer (implemented under type Binary)
     * 
     * I originally did not have this destructor. The compiler was calling delete on
     * 'Expr', but it is abstract and did not have a virtual destructor (runtime
     * polymorphism), so an error was thrown to avoid UB by inappropriately modifying memory.
     * 
     * \cite isocpp
     * \cite cppreference
     */
    virtual ~Expr() = default;
    
protected:
    virtual void accept(Visitor& visitor) = 0;
};

/**
 * 2 operands (expressions), 1 operator
 */
class Binary : public Expr {
public:
    Binary(std::unique_ptr<Expr> expr, Token op, std::unique_ptr<Expr> right);
    void accept(Visitor& visitor) override;

private:
    std::unique_ptr<Expr> m_left{}, m_right{};
    Token m_op{};
};

/**
 * 1 operand (expression), 1 operator
 * Can be left or right associative
 */
class Unary : public Expr {
public:
    Unary(Token op, std::unique_ptr<Expr> right);
    void accept(Visitor& visitor) override;
    
private:
    std::unique_ptr<Expr> m_expr{};
    Token m_op{};
};

/**
 * 1 operand (token/literal)
 */
class Primary : public Expr {
public:
    Primary(Token literal);
    void accept(Visitor& visitor) override;
    
private:
    Token m_literal{};
};

/**
 * 1 operand (expression)
 */
class Grouping : public Expr {
public:
    Grouping(std::unique_ptr<Expr> expr);
    void accept(Visitor& visitor) override;

private:
    std::unique_ptr<Expr> m_expr{};
};