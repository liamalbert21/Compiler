#include "Log.hpp"
#include "Settings.hpp"

Log& Log::instance() {
    static std::unique_ptr<Log> log{ new Log{} };
    return *log; 
}

bool Log::error(std::string&& message) {
    bool success{ false };
    
    // Checks whether there's another pair with the same key
    if (m_errors.size() <= Settings::Limits::max_errors && m_unique_messages.emplace(message).second) {
        m_errors.emplace(std::move(message));
        success = true;
    }

    return success;
}

bool Log::debug(std::string&& message) {
    bool success{ false };

    if (m_unique_messages.emplace(message).second) {
        m_debug.emplace(std::move(message));
        success = true;
    }

    return success;
}

void Log::dump(std::ostream& dos, std::ostream& eos) {
    const bool dump_debug{ !m_errors.empty() };

    while (!m_errors.empty()) {
        eos << m_errors.front() << '\n';
        m_errors.pop();
    }

    // Do not dump debug messages if the pipeline failed to compile the input
    if (dump_debug) {
        dos << m_debug.front() << '\n';
        m_debug.pop();
    }
}