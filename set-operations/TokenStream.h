#ifndef _TOKEN_STREAM_H
#define _TOKEN_STREAM_H

#include <string>

#include "Token.h"

class TokenStream {
public:
    TokenStream(std::string input);
    ~TokenStream();

    Token peek();
    bool isDone();
    Token discard();
    Token consume(Token::Type tokenType);

private:
    Token* parseNextToken();
    char currentCharacter();
    void advance();
    
    bool isCharacterWhitespace(char c);
    bool isCharacterLetter(char c);
    bool isCharacterDigit(char c);
    
    std::string _input;
    std::string::const_iterator _position;
    Token* _nextToken;
};

#endif // _TOKEN_STREAM_H
