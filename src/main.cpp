#include "Lexer.hpp"

int main() {
    // Maybe use a singleton class for settings? This could also work for an error handler
    constexpr int right_just{30};

    Lexer lexer{ std::ifstream{ PROJECT_ROOT_DIR "Code.txt" } };
    lexer.tokenize();
    lexer.printTokens(right_just);

    return 0;
}