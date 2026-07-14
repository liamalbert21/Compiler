#pragma once

#include "Token.hpp"

#include <fstream>
#include <vector>

class Lexer {
public:
    Lexer(std::ifstream&& input);
    void tokenize();
    void printTokens(int right_just) const;

private:
    struct NumberData {
        Token::Type type{};
        std::string value_str{};
    };

    Token getToken();
    Token generateNumericToken(Token::Type init_guess);
    Token::Type guessTokenType(char ch) const;
    NumberData getNumericTokenData(Token::Type final_guess);

    void advance();
    char extract();
    void extract(char& ch);
    char peek() const;
    void peek(char& ch) const;
    void prepareNextToken();
    bool isEOF() const;

    std::vector<Token> m_tokens{};
    std::string m_content{};
    std::string::iterator m_start{}, m_current{};
};
