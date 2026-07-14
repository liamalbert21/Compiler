#include "Lexer.hpp"

#include <iostream>
#include <iomanip>
#include <cassert>
#include <sstream>

Lexer::Lexer(std::ifstream&& input) {
    std::ostringstream oss{};
    oss << input.rdbuf();
    m_content = oss.str();

    assert(m_content.length() > 0);
    m_start = m_content.begin();
    m_current = m_content.begin();
}

void Lexer::tokenize() {
    m_tokens.clear();

    // "token" will be deemed false if its type is invalid
    while (Token&& token{getToken()}) {
        m_tokens.push_back(token);

        if (isEOF()) {
            return;
        }
    }

    throw std::runtime_error("ERROR: Invalid syntax!");
}

void Lexer::printTokens(int right_just) const {
    if (m_tokens.empty()) {
        std::cerr << "ERROR: Token data has yet to be acquired!\n";
        return;
    }

    for (const Token& token : m_tokens) {
        std::cout << std::setw(right_just);

        // Cannot send std::variant directly to cout buffer
        switch (token.type) {
            case Token::Type::INT:
                std::cout << std::get<int>(token.literal);
                break;
            case Token::Type::DOUBLE:
                std::cout << std::get<double>(token.literal);
                break;
            default:
                std::cout << std::get<char>(token.literal);
        }

        std::cout << '\r' << Token::toString(token.type) << '\n';
    }
}

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
        target.lexeme = first;
    }
    prepareNextToken();

    // Ignore whitespace via recursion
    return target.type == Token::Type::__WHITESPACE ? getToken() : target;
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

    return { data.type, literal, data.value_str };
}

Token::Type Lexer::guessTokenType(char ch) const {
    switch (ch) {
        // Double specifier
        case '.':
            return Token::Type::__SEPARATOR;

        // Grouping
        case '(':
            return Token::Type::LEFT_PAREN;
        case ')':
            return Token::Type::RIGHT_PAREN;
        case '[':
            return Token::Type::LEFT_BRACK;
        case ']':
            return Token::Type::RIGHT_BRACK;

        // Operators
        case '+':
            return Token::Type::PLUS;
        case '-':
            return Token::Type::MINUS;
        case '*':
            return Token::Type::STAR;
        case '/':
            return Token::Type::SLASH;
        case '!':
            return Token::Type::FACTORIAL;
        
        // Whitespace (will eventually ignore)
        case ' ':
            return Token::Type::__WHITESPACE;
        case '\n':
            return Token::Type::__WHITESPACE;
        case '\r':
            return Token::Type::__WHITESPACE;

        // Numbers
        default:
            if (std::isdigit(ch)) {
                return Token::Type::__DIGIT;
            }
    }

    return Token::Type::INVALID;
}

Lexer::NumberData Lexer::getNumericTokenData(Token::Type final_guess) {
    Token::Type type{ final_guess };
    
    while (!isEOF()) {
        Token::Type partial_type{ guessTokenType(peek()) };

        if (partial_type == Token::Type::__SEPARATOR) {
            // i.e. a separator was already encountered in the target token
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