#pragma once

#include <queue>
#include <unordered_set>
#include <string>
#include <iostream>

class Log {
public:
    static Log& instance();
    
    bool error(std::string&& message);
    void dump(std::ostream& os = std::cerr);

private:
    Log() = default;

    std::unordered_set<std::string> m_unique_messages{};
    std::queue<std::string> m_queue{};
};