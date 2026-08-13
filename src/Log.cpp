#include "Log.hpp"
#include "Settings.hpp"

#include <sstream>

// For timestamps...
// #include <chrono>

Log& Log::instance() {
    static std::unique_ptr<Log> log{ new Log{} };
    return *log; 
}

bool Log::error(std::string&& message) {
    bool success{ false };

    if (m_errors.size() <= Settings::Limits::max_errors && m_unique_errors.insert(message).second) {
        m_errors.emplace(std::move(message));
        success = true;
    }
    
    return success;
}

void Log::debug(std::string&& message, int right_just) {
    std::ostringstream oss{};
    oss << message << std::setw(right_just) /* << '[' << TIMESTAMP HERE << ']' */;
    m_debug.emplace(oss.str());
}

void Log::dump(std::ostream& dos, std::ostream& eos) {
    const bool dump_debug{ m_errors.empty() };

    // Do not dump debug messages if the pipeline failed to compile the input
    // In other words, only dump the debug log if no errors were produced
    if (dump_debug) {
        while (!m_debug.empty()) {
            dos << m_debug.front() << '\n';
            m_debug.pop();
        }
        dos << std::endl;
        return;
    }

    while (!m_errors.empty()) {
        eos << m_errors.front() << '\n';
        m_errors.pop();
    }
    eos << std::endl;
}