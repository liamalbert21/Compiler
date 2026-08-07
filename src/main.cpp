#include "Parser.hpp"
#include "Lexer.hpp"
#include "Log.hpp"
#include "Settings.hpp"

// Input configuration
#define FILE_INPUT          0
#define MANUAL_INPUT        1
#define MANUAL_INPUT_STR    "This input will fail :)"
#define INPUT_METHOD        FILE_INPUT

int main() {
    std::vector<Token> tokens{};
    bool success{};
    
// Using macros to avoid creating a default constructor for the lexer. In other
// words, I don't want lexer to be instantiatied within a conditional branch
#if INPUT_METHOD == FILE_INPUT
    Lexer lexer{ ROOT_DIR / Settings::File::input };
#elif INPUT_METHOD == MANUAL_INPUT 
    std::string input{ MANUAL_INPUT_STR };
    Lexer lexer{ std::move(input) };
#endif

    success = lexer.tokenize(tokens);
    if (!success) {
        Log::instance().dump();
        return -1;
    }

    lexer.printTokens(tokens, Settings::Text::right_just);
    Parser parser{ std::move(tokens) };

    success = parser.generateAST();
    if (!success) {
        Log::instance().dump();
        return -1;
    }

    parser.printAST();

    return 0;
}