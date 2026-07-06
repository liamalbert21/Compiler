#include "Lexer.hpp"

#include <iostream>
#include <iomanip>
#include <cassert>
#include <fstream>
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

    while (Token&& token{getToken()}) {
        if (token.type == Token::Type::__WHITESPACE) {
            continue;
        }
        m_tokens.push_back(token);

        if (isEOF()) {
            return;
        }
    }

    throw std::runtime_error("ERROR: Invalid character!");
}

void Lexer::printTokens() const {
    if (m_tokens.empty()) {
        std::cerr << "ERROR: Token data has yet to be acquired!\n";
        return;
    }

    for (const Token& token : m_tokens) {
        std::cout << std::setw(30);

        if (token.type == Token::Type::INT) {
            std::cout << std::get<int>(token.literal);
        } else if (token.type == Token::Type::DOUBLE) {
            std::cout << std::get<double>(token.literal);
        } else {
            std::cout << std::get<char>(token.literal);
        }

        std::cout << '\r' << Token::toString(token.type) << '\n';
    }
}

Token Lexer::getToken() {
    Token target{};
    const char first{extract()};
    Token::Type type{getPartialTokenType(first)};

    bool is_separator{type == Token::Type::__SEPARATOR};
    if (is_separator) {
        // Necessary to decremenet pointer to capture '.' again with getNumberData()
        backtrack();
    }

    if (type == Token::Type::__DIGIT || is_separator) {
        auto data{getNumberData()};
        type = data.type;

        if (type == Token::Type::INT) {
            target.literal = Literal{std::in_place_type<int>, std::stoi(data.value_str)};
        } else if (type == Token::Type::DOUBLE) {
            target.literal = Literal{std::in_place_type<double>, std::stod(data.value_str)};
        } else {
            throw std::runtime_error("CODE FAULT: A digit token was converted into something non-numeric!");
        }

    } else {
        target.literal = first;
        target.lexeme = first;
    }

    target.type = type;
    readyNextToken();
    return target;
}

Token::Type Lexer::getPartialTokenType(char ch) const {
    switch (ch) {
        // Handle symbols...
        case '.':
            return Token::Type::__SEPARATOR;
        case '(':
            return Token::Type::LEFT_PAREN;
        case ')':
            return Token::Type::RIGHT_PAREN;
        case '[':
            return Token::Type::LEFT_BRACK;
        case ']':
            return Token::Type::RIGHT_BRACK;
        case '+':
            return Token::Type::PLUS;
        case '-':
            return Token::Type::MINUS;
        case '*':
            return Token::Type::STAR;
        case '/':
            return Token::Type::SLASH;
        
        // Whitespace (will eventually ignore)
        case ' ':
            return Token::Type::__WHITESPACE;
        case '\n':
            return Token::Type::__WHITESPACE;
        case '\r':
            return Token::Type::__WHITESPACE;

        // Integers and doubles
        default:
            if (std::isdigit(ch)) {
                return Token::Type::__DIGIT;
            }
    }

    return Token::Type::INVALID;
}

Lexer::NumberData Lexer::getNumberData() {
    Token::Type type{Token::Type::INT};
    
    while (!isEOF()) {
        Token::Type partial_type{getPartialTokenType(peek())};

        if (partial_type == Token::Type::__SEPARATOR) {
            type = Token::Type::DOUBLE;
        } else if (partial_type != Token::Type::__DIGIT) {
            break;
        }

        advance();
    }

    return {type, std::string{m_start, m_current}};
}

void Lexer::advance() {
    ++m_current;
}

void Lexer::backtrack() {
    --m_current;
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

void Lexer::readyNextToken() {
    m_start = m_current;
}

bool Lexer::isEOF() const {
    return m_current >= m_content.end();
}