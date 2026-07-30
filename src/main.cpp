#include "Parser.hpp"
#include "Lexer.hpp"
#include "Settings.hpp"

int main() {
    Lexer lexer{ Settings::File::root_dir / Settings::File::input };
    std::vector<Token> tokens{ lexer.tokenize() };
    lexer.printTokens(tokens, Settings::Text::right_just);

    Parser parser{ std::move(tokens) };
    parser.generateAST();
    parser.printAST();

    return 0;
}