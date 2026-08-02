#include "Parser.hpp"
#include "Lexer.hpp"
#include "Log.hpp"
#include "Settings.hpp"

int main() {
    std::vector<Token> tokens{};

    Lexer lexer{ Settings::File::root_dir / Settings::File::input };
    lexer.tokenize(tokens);

    if (!tokens.empty()) {
        lexer.printTokens(tokens, Settings::Text::right_just);

        Parser parser{ std::move(tokens) };
        parser.generateAST();
        parser.printAST();
    }

    Log::instance().dump();

    return 0;
}