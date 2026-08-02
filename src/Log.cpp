#include "Log.hpp"

#include <memory>

Log& Log::instance() {
    static std::unique_ptr<Log> log{ new Log{} };
    return *log; 
}

bool Log::error(std::string&& message) {
    static std::size_t current{ 0 };

    // Checks whether there's another pair with the same key
    if (m_unique_messages.emplace(message, current++).second) {
        m_queue.emplace(std::move(message));
        return true;
    }

    return false;
}

void Log::dump(std::ostream& os) {
    while (!m_queue.empty()) {
        os << m_queue.front() << '\n';
        m_queue.pop();
    }
}