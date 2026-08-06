#pragma once

#include <queue>
#include <unordered_set>
#include <string>
#include <iostream>

class Log {
public:
    static Log& instance();
    
    bool error(std::string&& message);
    bool debug(std::string&& message);

    void dump(std::ostream& dos = std::cout, std::ostream& eos = std::cerr);

private:
    Log() = default;

    std::unordered_set<std::string> m_unique_messages{};
    std::queue<std::string> m_errors{}, m_debug{};
};