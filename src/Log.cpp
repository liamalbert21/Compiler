#include "Log.hpp"

#include <memory>

Log& Log::instance() {
    static std::unique_ptr<Log> log{ new Log{} };
    return *log; 
}

bool Log::error(std::string&& message) {
    // Checks whether there's another pair with the same key
    if (m_unique_messages.emplace(message).second) {
        m_queue.emplace(std::move(message));
        return true;
    }

    return false;
}

/**
 * @brief Dumps log contents to the provided stream
 * 
 * @note  My intention is that the log is dumped once per cycle. Developers can
 *        choose the stream to which the output is sent (std::cerr by default)
 *        in case they'd like to save the contents and store/use them elsewhere.
 *
 *        Dumping is extremely efficient (thanks to the queue) in case many
 *        messages have accumulated during the compilation process.
 *
 * @param os 
 */
void Log::dump(std::ostream& os) {
    while (!m_queue.empty()) {
        os << m_queue.front() << '\n';
        m_queue.pop();
    }
}