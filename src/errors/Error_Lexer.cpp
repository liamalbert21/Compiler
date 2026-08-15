#include "Lexer.hpp"

#include <sstream>

void Error<Lexer>::InvalidCharacter(Lexer& lexer) {
    auto description{ [](Context data) -> std::string {
        std::ostringstream oss{};

        oss << std::get<std::string_view>(data.first) << '\n'
            << std::setw(data.second + 1) << "^\n"
            << std::setw(data.second + 1) << "|\n";

        return oss.str();
    }};

    std::string message{ "Invalid character!" };
    lexer.ErrorWrapper(message, std::function<std::string(Context)>{ description });
}
