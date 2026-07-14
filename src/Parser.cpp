#include "Parser.hpp"

#include <cassert>

Parser::Parser(std::vector<Token> tokens) :
    m_tokens{tokens} {
        assert(m_tokens.size() > 0);
        m_start = m_tokens.begin();
        m_current = m_tokens.begin();
    }

void Parser::advance() {
    ++m_current;
}

void Parser::backtrack() {
    --m_current;
}

Token Parser::extract() {
    return *(m_current++);
}

void Parser::extract(Token& token) {
    token = *(m_current++);
}

Token Parser::peek() const {
    return *m_current;
}

void Parser::peek(Token& token) const {
    token = *m_current;
}

void Parser::prepareNextToken() {
    m_start = m_current;
}

bool Parser::isEOF() const {
    return m_current >= m_tokens.end();
}