#include "Parser.hpp"
#include "Lexer.hpp"
#include "Log.hpp"
#include "Settings.hpp"

// Input configuration
#define FILE_INPUT          0
#define MANUAL_INPUT        1
#define MANUAL_INPUT_STR    "1 2"
#define INPUT_METHOD        FILE_INPUT

int main() {
    std::vector<Token> tokens{};

#if INPUT_METHOD == FILE_INPUT
    Lexer lexer{ ROOT_DIR / Settings::File::input };
#elif INPUT_METHOD == MANUAL_INPUT
    std::string input{ MANUAL_INPUT_STR };
    Lexer lexer{ std::move(input) };
#endif

    Log::instance().debug("Tokenizing the input...");
    if (!lexer.tokenize(tokens)) {
        Log::instance().dump();
        return -1;
    }
    Log::instance().debug("Success!");
    
    Parser parser{ tokens };

    Log::instance().debug("Constructing the AST...");
    if (!parser.generateAST()) {
        Log::instance().dump();
        return -1;
    }
    Log::instance().debug("Success!");

    Log::instance().dump();
    lexer.printTokens(tokens, Settings::Text::right_just);
    parser.printAST();

    return 0;
}