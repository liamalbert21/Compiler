#pragma once

#include "Token.hpp"

#include <memory>

class Binary; class Unary; class Primary; class Grouping;

// Because every instance that retes to Expr must be a pointer (as Expr is
// abstract), children must inherit Expr publicly to be able to match the Expr
// type when recursively binding child nodes that need to be interpreted as
// pointers to Expr.
class Expr {
public:
    /**
     * @brief Identifies the side on which an operand lies relative to its
     *        corresponding oeprator
     *
     */
    enum class OperandSide {
        LEFT, RIGHT, UNKNOWN
    };

    /**
     * @brief Implementing the visitor pattern to provide 
     * 
     */
    class Visitor {
    public:
        virtual void visit(Binary& binary)     const = 0;
        virtual void visit(Unary& unary)       const = 0;
        virtual void visit(Primary& primary)   const = 0;
        virtual void visit(Grouping& grouping) const = 0;
    };

    class Eval : public Visitor {
    public:
        void visit(Binary& binary)     const override;
        void visit(Unary& unary)       const override;
        void visit(Primary& primary)   const override;
        void visit(Grouping& grouping) const override;
    };

    class Print : public Visitor {
    public:
        void visit(Binary& binary)     const override;
        void visit(Unary& unary)       const override;
        void visit(Primary& primary)   const override;
        void visit(Grouping& grouping) const override;
    };

    virtual void accept(const Visitor& visitor) = 0;
    virtual ~Expr() = default;
};

// 2 operands (expressions), 1 operator
class Binary : public Expr {
    friend class Expr;

public:
    Binary(std::unique_ptr<Expr> expr, Token op, std::unique_ptr<Expr> right);
    void accept(const Visitor& visitor) override;

private:
    std::unique_ptr<Expr> m_left{}, m_right{};
    Token m_op{};
};

// 1 operand (expression), 1 operator
// Can be left or right associative
class Unary : public Expr {
    friend class Expr;

public:
    Unary(Token op, std::unique_ptr<Expr> expr);
    void accept(const Visitor& visitor) override;
    
private:
    std::unique_ptr<Expr> m_expr{};
    Token m_op{};
};

// 1 operand (token/literal)
class Primary : public Expr {
    friend class Expr;

public:
    Primary(const Token& token);
    void accept(const Visitor& visitor) override;
    
private:
    Token m_value{};
};

// 1 operand (expression)
class Grouping : public Expr {
    friend class Expr;

public:
    Grouping(std::unique_ptr<Expr> expr);
    void accept(const Visitor& visitor) override;

private:
    std::unique_ptr<Expr> m_expr{};
};