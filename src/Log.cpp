#include "Log.hpp"
#include "Settings.hpp"

Log& Log::instance() {
    static std::unique_ptr<Log> log{ new Log{} };
    return *log; 
}

bool Log::error(std::string&& message) {
    return m_errors.size() <= Settings::Limits::max_errors && m_unique_messages.emplace(message).second;
}

bool Log::debug(std::string&& message) {
    return m_unique_messages.emplace(message).second;
}

void Log::dump(std::ostream& dos, std::ostream& eos) {
    const bool dump_debug{ !m_errors.empty() };

    while (!m_errors.empty()) {
        eos << m_errors.front() << '\n';
        m_errors.pop();
    }

    // Do not dump debug messages if the pipeline failed to compile the input
    // In other words, only dump the debug log if no errors were produced
    if (dump_debug) {
        while (!m_debug.empty()) {
            dos << m_debug.front() << '\n';
            m_debug.pop();
        }
    }
}