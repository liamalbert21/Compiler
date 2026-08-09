#pragma once

#include "Pipeline.hpp"
#include "Token.hpp"

#include <filesystem>

class Lexer : private Pipeline {
public:
    Lexer(std::string content);
    Lexer(const std::filesystem::path& input);

    std::pair<std::vector<Token>, bool> tokenize();
    bool tokenize(std::vector<Token>& tokens);
    void printTokens(const std::vector<Token>& tokens, std::size_t right_just) const;

    void ErrorWrapper(std::string_view start, std::function<std::string(Context)> func) override;

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
    std::string::iterator m_start{}, m_current{};

};

#include "Error.hpp"

template <>
class Error<Lexer> {
public:
    static void InvalidCharacter(Lexer& lexer);
};