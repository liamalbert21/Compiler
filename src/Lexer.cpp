#include "Lexer.hpp"
#include "Log.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>

namespace fs = std::filesystem;

void Lexer::initConditionalMembers() {
    if (m_content.length()) {
        m_start = m_content.begin();
        m_current = m_content.begin();
    }
}

/**
 * @brief Construct a new Lexer:: Lexer object
 *
 * @note  Moving content during member initialization permits both move and copy
 *        construction with one string overload and optimized efficientcy.
 *        Either pass an rvalue and invoke the move constructor for content or
 *        pass an lvalue and perform copy construction on content.
 *
 *        In either case, m_content is initialized via move construction. 
 *
 * @param content 
 */
Lexer::Lexer(std::string content) :
    m_state{ State::INIT }, m_content{ std::move(content) } {
        initConditionalMembers();
    }

Lexer::Lexer(const fs::path& input) :
    m_state{ State::INIT } {
        std::ostringstream oss{};
        oss << std::ifstream{ input }.rdbuf();
        m_content = oss.str();

        initConditionalMembers();
    }

std::pair<std::vector<Token>, bool> Lexer::tokenize() {
    std::vector<Token> tokens;

    // "token" will be deemed false if its type is invalid
    while (Token&& token{ getToken() }) {
        // Ignore whitespace
        if (token.type != Token::Type::__WHITESPACE) {
            tokens.push_back(token);
        }
        if (isEOF()) {
            return { tokens, true };
        }
    }

    Error<Lexer>::InvalidCharacter(*this);
    tokens.clear();
    return { tokens, false };
}

bool Lexer::tokenize(std::vector<Token>& tokens) {
    // auto works here because this function requires an argument (tokens),
    // while the overload accepts none
    auto result{ tokenize() };
    tokens = std::move(result.first);
    return result.second;
}

void Lexer::printTokens(const std::vector<Token>& tokens, std::size_t right_just) const {
    if (tokens.empty()) {
        return;
    }

    // Might want to eventually customize where this is sent. Could throw a
    // stream variable into settings or pass an additional stream argument
    for (const Token& token : tokens) {
        std::cout << std::setw(right_just) << token << '\r' << Token::ToString::Type(token.type) << '\n';
    }
}

// I originally wanted to handle whitespace here via recursion. The solution
// looked clean as well but surely would have caused stack overflow when
// provided inputs with many contiguous whitespace characters.
Token Lexer::getToken() {
    Token target{};
    const char first{ extract() };
    target.type = guessTokenType(first);

    // Handle numbers
    if (target.type == Token::Type::__DIGIT || target.type == Token::Type::__SEPARATOR) {
        target = generateNumericToken(target.type);
    }
    else {
        target.literal = first;
    }
    prepareNextToken();

    return target;
}

Token Lexer::generateNumericToken(Token::Type init_guess) {
    // Redirect the initial guess to a numeric type
    Token::Type new_guess{ init_guess == Token::Type::__SEPARATOR ? Token::Type::DOUBLE : Token::Type::INT };

    const auto data{ getNumericTokenData(new_guess) };
    Literal literal{};

    if (data.type == Token::Type::INT) {
        literal = Literal{ std::in_place_type<int>, std::stoi(data.value_str) };
    }
    else if (data.type == Token::Type::DOUBLE) {
        literal = Literal{ std::in_place_type<double>, std::stod(data.value_str) };
    }

    // Strictly for debugging
    else if (data.type != Token::Type::INVALID) {
        throw std::runtime_error("CODE FAULT: A digit token was converted into something non-numeric!");
    }

    return { data.type, literal };
}

Lexer::Number Lexer::getNumericTokenData(Token::Type final_guess) {
    Token::Type type{ final_guess };
    
    while (!isEOF()) {
        Token::Type partial_type{ guessTokenType(peek()) };

        if (partial_type == Token::Type::__SEPARATOR) {
            // A separator was already encountered in the target token
            if (type == Token::Type::DOUBLE) {
                return { Token::Type::INVALID, "" };
            }
            type = Token::Type::DOUBLE;
        }
        else if (partial_type != Token::Type::__DIGIT) {
            break;
        }

        advance();
    }

    return { type, std::string{ m_start, m_current } };
}

Token::Type Lexer::guessTokenType(char ch) const {
    using enum Token::Type;

    switch (ch) {
        // Double specifier
        case '.':
            return __SEPARATOR;

        // Grouping
        case '(':
            return LEFT_PAREN;
        case ')':
            return RIGHT_PAREN;
        case '[':
            return LEFT_BRACK;
        case ']':
            return RIGHT_BRACK;

        // Operators
        case '+':
            return PLUS;
        case '-':
            return MINUS;
        case '*':
            return STAR;
        case '/':
            return SLASH;
        case '!':
            return FACTORIAL;
        
        // Whitespace (will eventually ignore) \n and \r may later be changed if
        // I support multiple lines
        case ' ':
            return __WHITESPACE;
        case '\n':
            return __WHITESPACE;
        case '\r':
            return __WHITESPACE;

        // Numbers
        default:
            if (std::isdigit(ch)) {
                return __DIGIT;
            }
    }

    return INVALID;
}

void Lexer::advance() {
    ++m_current;
}

char Lexer::extract() {
    return *(m_current++);
}

void Lexer::extract(char& ch) {
    ch = *(m_current++);
}

char Lexer::peek() const {
    return *m_current;
}

void Lexer::peek(char& ch) const {
    ch = *m_current;
}

void Lexer::prepareNextToken() {
    m_start = m_current;
}

bool Lexer::isEOF() const {
    return m_current == m_content.end();
}

// Considering writing a default implementation for ErrorWrapper that takes a
// context and generates the associated error message, before setting m_state to
// fail.
void Lexer::ErrorWrapper(
    std::string_view start,
    std::function<std::string(Context)> func
) {
    std::ostringstream message{};
    const std::size_t width{ start.length() };

    message << start << '\n'
            << std::string(width, '-') << '\n'
            << func({ m_content, static_cast<std::size_t>(m_current - m_content.begin()) })
            << std::string(width, '-');

    Log::instance().error(message.str());
    m_state = State::FAIL;
}