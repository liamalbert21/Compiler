#include "Parser.hpp"

#include <sstream>

void Error<Parser>::ExpectedExpression(Parser& parser, Expr::OperandSide side, const Token& target) {
    auto description{ [=](Context data) -> std::string {
        std::ostringstream oss{};
        auto start{ std::get<std::span<const Token>>(data.first).begin() };

        oss << "* At token instance: "
            << std::count_if(
                    start,
                    start + data.second + 1,
                    [=](const Token& token) { return token.type == target.type; }
                )
            << '\n';

        return oss.str();
    }};

    std::ostringstream message{};
    message << "Expected expression "
            << (
                    side == Expr::OperandSide::LEFT  ? "before " : 
                    side == Expr::OperandSide::RIGHT ? "after "  : "before/after "
                )
            << '\'' << target << '\'';

    parser.ErrorWrapper(message.str(), std::function<std::string(Context)>{ description });
}

void Error<Parser>::ExpectedOperator(Parser& parser) {
    auto description{ [](Context data) -> std::string {
        std::ostringstream oss{};
        oss << "* At token position: " << data.second + 1 << '\n';
        return oss.str();
    }};

    std::string message{ "Expected operator!" };
    parser.ErrorWrapper(message, std::function<std::string(Context)>{ description });
}

void Error<Parser>::ExpectedClosingGroup(Parser& parser, Token::Type type) {
    auto description{ [](Context data) -> std::string {
        std::ostringstream oss{};
        oss << "* At token position: " << data.second + 1 << '\n';
        return oss.str();
    }};

    std::ostringstream message{};
    message << "Expected " << Token::ToString::Type(type) << '!';

    parser.ErrorWrapper(message.str(), std::function<std::string(Context)>{ description });
}