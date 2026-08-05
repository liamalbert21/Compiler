#pragma once

#include "Subsystem.hpp"
#include "Error.hpp"
#include "Expression.hpp"

#include <vector>
#include <optional>

class Parser : private Subsystem {
    friend struct Error<Parser>;

public:
    Parser(std::vector<Token> tokens);

    bool generateAST();
    void printAST() const;

private:
    struct MetaIn {
        std::size_t input_length{};
        std::vector<Token>::const_iterator current{};
    };

    std::optional<Token>  matchTokens(std::initializer_list<Token::Type> types);

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
    MetaIn m_metadata{};
    std::vector<Token> m_tokens{};
    std::unique_ptr<Expr> m_ast{};
};