#include "Lexer.hpp"

#include <fstream>

int main() {
    Lexer lexer{std::ifstream{PROJECT_ROOT_DIR "Code.txt"}};
    lexer.tokenize();
    lexer.printTokens();
    return 0;
}