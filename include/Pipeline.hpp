#pragma once

#include <functional>

struct Token;

using Content = std::variant<std::string_view, const std::vector<Token>*>;
using Context = std::pair<Content, std::size_t>;

/**
 * @brief Subsystems (pipelines) inherit this class privately to
 *           a) Access a state varaible without having to redefine it or provide
 *              a global entrypoint, and
 *           b) Acquire the ability to implement structured errors (see
 *              Error.hpp)
 *
 *        Include Pipeline.hpp if you only want access to the state variables
 *        for a particular pipeline, as Error.hpp already does so
 * 
 */
class Pipeline {
protected:
    enum class State {
        INIT, OKAY, FAIL
    };

    /**
     * @brief 
     * 
     * @param func 
     * @param args 
     */
    virtual void ErrorWrapper(std::string_view start, std::function<std::string(Context)> func) = 0;
};