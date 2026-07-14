#pragma once

#include "Token.hpp"

#include <vector>
#include <iterator>

class Parser {
public:
    Parser(std::vector<Token> tokens);

private:
    void advance();
    void backtrack();
    Token extract();
    void extract(Token& token);
    Token peek() const;
    void peek(Token& token) const;
    void prepareNextToken();
    bool isEOF() const;

    std::vector<Token> m_tokens{};
    std::vector<Token>::iterator m_start{}, m_current{};
};