#pragma once

#include "Pipeline.hpp"
#include "Expression.hpp"

#define TOKENS std::vector<Token>

class Parser : public Pipeline {
public:
    Parser(TOKENS tokens);

    bool generateAST();
    void printAST() const;

    void ErrorWrapper(
        std::string_view start,
        std::function<std::string(std::pair<Content, std::size_t>)> func
    ) override;

private:
    std::optional<Token> matchTokens(std::initializer_list<Token::Type> types);
    Expr::OperandSide getMissingOperandSide(std::unique_ptr<Expr>& left, std::unique_ptr<Expr>& right);

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();

    struct unary {
        static std::unique_ptr<Expr> right(Parser& parser);
        static std::unique_ptr<Expr> left(Parser& parser);
    };

    std::unique_ptr<Expr> primary();

    void  advance();
    Token extract();
    void  extract(Token& token);
    Token peek() const;
    void  peek(Token& token) const;
    bool  isEOF() const;

    State m_state{};
    TOKENS m_tokens{};
    TOKENS::iterator m_current{};
    std::unique_ptr<Expr> m_ast{};
};

#include "Error.hpp"

template<>
class Error<Parser> {
public:
    static void ExpectedExpression(Parser& parser, Expr::OperandSide side, Token::Type type);
    static void ExpectedOperand(Parser& parser);
};

#undef TOKENS