#pragma once

#include <queue>
#include <unordered_set>
#include <string>
#include <iostream>

class Log {
public:
    static Log& instance();
    
    bool error(std::string&& message);
    
    // Change right_just to not have default parameter value once timestamps are
    // implemented
    void debug(std::string&& message, int right_just = 0);

    void dump(std::ostream& dos = std::cout, std::ostream& eos = std::cerr);

private:
    Log() = default;

    std::unordered_set<std::string> m_unique_errors{};
    std::queue<std::string> m_errors{}, m_debug{};
};