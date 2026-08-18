#include <catch2/catch_test_macros.hpp>

#include "Lexer.hpp"
#include "Parser.hpp"

#include <vector>

using enum Token::Type;

bool isValidLexerOutput(std::string& expression, const std::vector<Token::Type>& valid) {
    Lexer lexer{ std::move(expression) };
    auto out{ lexer.tokenize() };

    if (!out.second) {
        return false;
    }

    for (std::size_t i{ 0 }; i < out.first.size(); ++i) {
        if (out.first[i].type != valid[i]) {
            return false;
        }
    }

    return true;
}

TEST_CASE( "Empty expression", "[Lexer]" ) {
    Lexer lexer{ std::string{ "" } };
    auto result{ lexer.tokenize() };
    REQUIRE(!result.second);
}

TEST_CASE( "Simple expression", "[Lexer]" ) {
    std::string expression{"5! + 12 * 3 - 8 / 2" };
    std::vector<Token::Type> expected {
        INT, FACTORIAL, PLUS, INT, STAR,
        INT, MINUS, INT, SLASH, INT
    };
    REQUIRE(isValidLexerOutput(expression, expected));
}

TEST_CASE( "Long expression", "[Lexer]" ) {
    std::string expression{"[ ( ( (4! + 6) * (3.1 - 1) ) / (5! - 100) ) ] - [ (7! - 5000) / ( 2 * (10) ) ] + [ 3! * ( 8 - [ 2 + (5 - 3)! ] ) ]" };
    std::vector<Token::Type> expected {
        LEFT_BRACK, LEFT_PAREN, LEFT_PAREN, LEFT_PAREN, INT,
        FACTORIAL, PLUS, INT, RIGHT_PAREN, STAR, LEFT_PAREN,
        DOUBLE, MINUS, INT, RIGHT_PAREN, RIGHT_PAREN,
        SLASH, LEFT_PAREN, INT, FACTORIAL, MINUS,
        INT, RIGHT_PAREN, RIGHT_PAREN, RIGHT_BRACK, MINUS,
        LEFT_BRACK, LEFT_PAREN, INT, FACTORIAL, MINUS,
        INT, RIGHT_PAREN, SLASH, LEFT_PAREN, INT,
        STAR, LEFT_PAREN, INT, RIGHT_PAREN, RIGHT_PAREN,
        RIGHT_BRACK, PLUS, LEFT_BRACK, INT, FACTORIAL,
        STAR, LEFT_PAREN, INT, MINUS, LEFT_BRACK,
        INT, PLUS, LEFT_PAREN, INT, MINUS,
        INT, RIGHT_PAREN, FACTORIAL, RIGHT_BRACK, RIGHT_PAREN,
        RIGHT_BRACK
    };

    REQUIRE(isValidLexerOutput(expression, expected));
}

TEST_CASE( "Invalid expression", "[Lexer]" ) {
    std::string expression{ "4.31a3" };
    REQUIRE(!isValidLexerOutput(expression, { INT }));
}