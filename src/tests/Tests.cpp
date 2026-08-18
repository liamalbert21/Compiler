#include <catch2/catch_test_macros.hpp>

#include "Lexer.hpp"
#include "Parser.hpp"

#include <iostream>
#include <cctype>

using enum Token::Type;

bool isValidLexerOutput(std::string& expression, const std::vector<Token::Type>& valid) {
    Lexer lexer{ std::move(expression) };
    auto out{ lexer.tokenize() };

    if (!out.second || out.first.size() != valid.size()) {
        return false;
    }

    for (std::size_t i{ 0 }; i < valid.size(); ++i) {
        if (out.first[i].type != valid[i]) {
            return false;
        }
    }
    return true;
}

std::string& removeWS(std::string&& str) {
    auto check{ [](char ch) { return std::isspace(ch); } };
    std::erase_if(str, check);
    return str;
}

std::string getPrintedAST(std::string&& expression) {
    Lexer lexer{ std::move(expression) };
    Parser parser{ std::move(lexer.tokenize().first) };
    
    if (!parser.generateAST()) {
        return "";
    }

    std::stringstream local{};
    auto cout_ptr{ std::cout.rdbuf() };

    std::cout.rdbuf(local.rdbuf());
    parser.printAST();
    std::cout.rdbuf(cout_ptr);

    return local.str();
}

TEST_CASE( "Empty input", "[Lexer]" ) {
    Lexer lexer{ std::string{ "" } };
    auto result{ lexer.tokenize() };
    REQUIRE(!result.second);
}

TEST_CASE( "Short valid expression", "[Lexer]" ) {
    std::string expression{ "5! + 12 * 3 - 8 / 2" };

    std::vector<Token::Type> expected
    {
        INT, FACTORIAL, PLUS, INT, STAR,
        INT, MINUS, INT, SLASH, INT
    };

    REQUIRE(isValidLexerOutput(expression, expected));
}

TEST_CASE( "Long valid expression", "[Lexer]" ) {
    std::string expression
    {
        "[ ( ( (4! + 6) * (3.1 - 1) ) / (5! - 100) ) ] - [ (7! - 5000)"
        "/ ( 2 * (10) ) ] + [ 3! * ( 8 - [ 2 + (5 - 3)! ] ) ]"
    };
    
    std::vector<Token::Type> expected
    {
        LEFT_BRACK, LEFT_PAREN, LEFT_PAREN, LEFT_PAREN, INT,
        FACTORIAL, PLUS, INT, RIGHT_PAREN, STAR, LEFT_PAREN,
        DOUBLE, MINUS, INT, RIGHT_PAREN, RIGHT_PAREN, SLASH,
        LEFT_PAREN, INT, FACTORIAL, MINUS, INT, RIGHT_PAREN,
        RIGHT_PAREN, RIGHT_BRACK, MINUS, LEFT_BRACK,
        LEFT_PAREN, INT, FACTORIAL, MINUS, INT, RIGHT_PAREN,
        SLASH, LEFT_PAREN, INT, STAR, LEFT_PAREN, INT,
        RIGHT_PAREN, RIGHT_PAREN, RIGHT_BRACK, PLUS, LEFT_BRACK,
        INT, FACTORIAL, STAR, LEFT_PAREN, INT, MINUS, LEFT_BRACK,
        INT, PLUS, LEFT_PAREN, INT, MINUS, INT, RIGHT_PAREN,
        FACTORIAL, RIGHT_BRACK, RIGHT_PAREN, RIGHT_BRACK
    };

    REQUIRE(isValidLexerOutput(expression, expected));
}

TEST_CASE( "Invalid expression", "[Lexer]" ) {
    std::string expression{ "4.31a3" };
    REQUIRE(!isValidLexerOutput(expression, { INVALID }));
}

TEST_CASE( "Empty input", "[Parser]" ) {
    Parser parser{ {} };
    REQUIRE(!parser.generateAST());
}

TEST_CASE( "Short valid expression", "[Parser]" ) {
    REQUIRE(removeWS(getPrintedAST("5! + 12 * 3 - 8 / 2")) == removeWS("- + ! 5 * 12 3 / 8 2"));
}

TEST_CASE( "Long valid expression", "[Parser]" ) {
    std::string expression
    {
        "[ ( ( (4! + 6) * (3.1 - 1) ) / (5! - 100) ) ] - [ (7! - 5000)"
        "/ ( 2 * (10) ) ] + [ 3! * ( 8 - [ 2 + (5 - 3)! ] ) ]"
    };

    std::string expected
    {
        "+ - { { / { * { + ! 4 6 } { - 3.1 1 } } { - ! 5 100 } } } { / { - ! 7 5000 }"
        "{ * 2 { 10 } } } { * ! 3 { - 8 { + 2 ! { - 5 3 } } } }"
    };

    REQUIRE(removeWS(getPrintedAST(std::move(expression))) == removeWS(std::move(expected)));
}

TEST_CASE( "Invalid expression", "[Parser]" ) {
    REQUIRE(getPrintedAST("[ 1 2 + ]").empty());
}