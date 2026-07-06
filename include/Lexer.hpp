#pragma once

#include "Token.hpp"

#include <vector>

class Lexer {
public:
    Lexer(std::ifstream&& input);
    void tokenize();
    void printTokens() const;

private:
    struct NumberData {
        Token::Type type{};
        std::string value_str{};
    };

    Token getToken();
    Token::Type getPartialTokenType(char ch) const;
    NumberData getNumberData();

    void advance();
    void backtrack();
    char extract();
    void extract(char& ch);
    char peek() const;
    void peek(char& ch) const;
    void readyNextToken();
    bool isEOF() const;

    std::vector<Token> m_tokens{};
    std::string m_content{};
    std::string::iterator m_start{}, m_current{};
};
