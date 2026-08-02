#pragma once

#include <queue>
#include <unordered_map>
#include <string>
#include <iostream>

class Log {
public:
    static Log& instance();
    
    bool error(std::string&& message);
    void dump(std::ostream& os = std::cerr);

private:
    Log() = default;

    std::unordered_map<std::string, std::size_t> m_unique_messages{};
    std::queue<std::string> m_queue{};
};