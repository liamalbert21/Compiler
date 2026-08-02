#pragma once

#include "Error.hpp"
#include "Token.hpp"

#include <filesystem>
#include <vector>

class Lexer {
    friend struct Error<Lexer>;

public:
    Lexer(const std::filesystem::path& input);

    std::vector<Token> tokenize();
    void tokenize(std::vector<Token>& tokens);
    void printTokens(const std::vector<Token>& tokens, std::size_t right_just) const;

private:
    struct Number {
        Token::Type type{};
        std::string value_str{};
    };

    Token getToken();
    Token generateNumericToken(Token::Type init_guess);
    Token::Type guessTokenType(char ch) const;
    Number getNumericTokenData(Token::Type final_guess);

    void advance();
    char extract();
    void extract(char& ch);
    char peek() const;
    void peek(char& ch) const;
    void prepareNextToken();
    bool isEOF() const;

    std::string m_content{};
    std::string::const_iterator m_start{}, m_current{};
};