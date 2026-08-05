#include "Parser.hpp"
#include "Lexer.hpp"
#include "Log.hpp"
#include "Settings.hpp"

// Error codes
#define LEXER_FAIL      -1
#define PARSER_FAIL     -2

// Input configuration
#define FILE_INPUT      0
#define MANUAL_INPUT    1
#define INPUT_METHOD    MANUAL_INPUT

int main() {
    std::vector<Token> tokens{};
    bool success{};

#if INPUT_METHOD == FILE_INPUT
    Lexer lexer{ Settings::File::root_dir / Settings::File::input };
#elif INPUT_METHOD == MANUAL_INPUT
    std::string input{ "1 + 2 + 3 + 4" };
    Lexer lexer{ std::move(input) };
#endif

    success = lexer.tokenize(tokens);
    if (!success) {
        Log::instance().dump();
        return LEXER_FAIL;
    }

    lexer.printTokens(tokens, Settings::Text::right_just);
    Parser parser{ std::move(tokens) };

    success = parser.generateAST();
    if (!success) {
        Log::instance().dump();
        return PARSER_FAIL;
    }

    parser.printAST();

    return 0;
}