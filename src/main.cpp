#include "Parser.hpp"
#include "Lexer.hpp"
#include "Settings.hpp"

int main() {
    Lexer lexer{ std::ifstream{ PROJECT_ROOT_DIR "Code.txt" } };
    lexer.tokenize();
    lexer.printTokens(Settings::Text::RIGHT_JUST);

    Parser parser{ lexer.getTokens() };
    parser.generateAST();
    parser.printAST();

    return 0;
}