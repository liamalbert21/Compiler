#pragma once

#include "Error.hpp"
#include "Token.hpp"

#include <filesystem>
#include <vector>

class Lexer : public Pipeline {
    friend struct Error<Lexer>;

public:
    Lexer(std::string content);
    Lexer(const std::filesystem::path& input);

    std::pair<std::vector<Token>, bool> tokenize();
    bool tokenize(std::vector<Token>& tokens);
    void printTokens(const std::vector<Token>& tokens, std::size_t right_just) const;

private:
    struct Number {
        Token::Type type{};
        std::string value_str{};
    };

    void initConditionalMembers();

    Token getToken();
    Token generateNumericToken(Token::Type init_guess);
    Number getNumericTokenData(Token::Type final_guess);

    Token::Type guessTokenType(char ch) const;

    void advance();
    char extract();
    void extract(char& ch);
    char peek() const;
    void peek(char& ch) const;
    void prepareNextToken();
    bool isEOF() const;

    State m_state{};
    std::string m_content{};
    std::string::const_iterator m_start{}, m_current{};
};

template <>
struct Error<Lexer> {
    static std::string InvalidCharacter(const Lexer& lexer);
};