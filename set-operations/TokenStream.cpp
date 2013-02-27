#include "TokenStream.h"

#include <sstream>
#include <iostream>

TokenStream::TokenStream(std::string input) :
    _input(input),
    _position(_input.begin()),
    _nextToken(NULL)
{ /* void */ }

TokenStream::~TokenStream() {
    if (_nextToken) delete _nextToken;
}

Token TokenStream::peek() {
    if (!_nextToken) {
        if (isDone()) {
            std::cerr << "Unexpected end of input stream." << std::endl;
            throw std::exception();
        }
        _nextToken = parseNextToken();
    }
    return *_nextToken;
}

bool TokenStream::isDone() {
    return _position == _input.end();
}

Token TokenStream::discard() {
    Token result = peek();
    delete _nextToken;
    _nextToken = NULL;
    return result;
}

Token TokenStream::consume(Token::Type tokenType) {
    if (peek().type() != tokenType) {
        std::cerr
            << "CRITICAL: Expected token '"
            << Token::typeString(tokenType)
            << "', but got '"
            << peek().typeString()
            << "' ('"
            << peek().string()
            << "') instead."
            << std::endl;
        std::cerr
            << "\t=> context: "
            << _input.substr(0, _position - _input.begin())
            << " <<<HERE<<< "
            << _input.substr(_position - _input.begin())
            << std::endl;
        throw std::exception();
    }
    return discard();
}

Token* TokenStream::parseNextToken() {
    // skip whitespace
    while (isCharacterWhitespace(currentCharacter())) advance();

    if (currentCharacter() == '(') {
        advance();
        return new Token(Token::LEFT_PARENTHESIS, "(");
    } else if (currentCharacter() == ')') {
        advance();
        return new Token(Token::RIGHT_PARENTHESIS, ")");
    } else if (currentCharacter() == ',') {
        advance();
        return new Token(Token::COMMA, ",");
    } else if (isCharacterLetter(currentCharacter())) {
        // accumulate potential keyword
        std::stringstream accumulator;
        while (isCharacterLetter(currentCharacter())) {
            accumulator << currentCharacter();
            advance();
        }
        std::string word = accumulator.str();
        if (word == "POLYGON") return new Token(Token::KEYWORD_POLYGON, word);
        else if (word == "MULTIPOLYGON") return new Token(Token::KEYWORD_MULTIPOLYGON, word);
        else if (word == "EMPTY") return new Token(Token::KEYWORD_EMPTY, word);
        else return new Token(Token::UNKNOWN, word);
    } else if (currentCharacter() == '-' || currentCharacter() == '.' || isCharacterDigit(currentCharacter())) {
        // accumulate unknown token
        std::stringstream accumulator;
        if (currentCharacter() == '-') {
            accumulator << currentCharacter();
            advance();
        }
        while (isCharacterDigit(currentCharacter())) {
            accumulator << currentCharacter();
            advance();
        }
        if (currentCharacter() == '.') {
            accumulator << currentCharacter();
            advance();
        }
        while (isCharacterDigit(currentCharacter())) {
            accumulator << currentCharacter();
            advance();
        }
        std::string word = accumulator.str();
        return new Token(Token::NUMBER, word);
    } else {
        // accumulate unknown token
        std::stringstream accumulator;
        while (!isCharacterWhitespace(currentCharacter())) {
            accumulator << currentCharacter();
            advance();
            if (isDone()) break;
        }
        std::string word = accumulator.str();
        return new Token(Token::UNKNOWN, word);
    }
}

char TokenStream::currentCharacter() {
    return *_position;
}

void TokenStream::advance() {
    if (isDone()) {
        std::cerr << "Unexpected end of input stream." << std::endl;
        throw std::exception();
    }
    ++_position;
}

bool TokenStream::isCharacterWhitespace(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

bool TokenStream::isCharacterLetter(char c) {
    return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z';
}

bool TokenStream::isCharacterDigit(char c) {
    return c >= '0' && c <= '9';
}
