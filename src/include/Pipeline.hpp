#pragma once

#include <string_view>
#include <functional>
#include <span>

struct Token;

using Content = std::variant<std::string_view, std::span<const Token>>;
using Context = std::pair<Content, std::size_t>;

class Pipeline {
protected:
    enum class State {
        INIT, OKAY, FAIL
    };

    virtual void ErrorWrapper(
        std::string_view start,
        std::function<std::string(Context)> func
    ) = 0;
};