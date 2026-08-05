#pragma once

#include <queue>
#include <unordered_set>
#include <string>
#include <iostream>

class Log {
public:
    static Log& instance();
    
    bool error(std::string&& message);

    // My intention is that the log is dumped once per cycle. Developers can
    // choose the stream to which the output is sent (std::cerr by default)
    // in case they'd like to save the contents and store/use them elsewhere.
    // Dumping is extremely efficient (thanks to the queue) in case many
    // messages have accumulated during the compilation process.
    void dump(std::ostream& os = std::cerr);

private:
    Log() = default;

    std::unordered_set<std::string> m_unique_messages{};
    std::queue<std::string> m_queue{};
};