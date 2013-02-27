#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>

class Token {
public:
    enum Type {
        UNKNOWN,
        NUMBER,
        LEFT_PARENTHESIS,
        RIGHT_PARENTHESIS,
        COMMA,
        KEYWORD_POLYGON,
        KEYWORD_MULTIPOLYGON,
        KEYWORD_EMPTY
    };
    
    static std::string typeString(Token::Type tokenType);
    
    Token(Token::Type tokenType, std::string tokenString);
    
    Token::Type type() const;
    std::string typeString() const;
    std::string string() const;
    double asDouble() const;

private:
    Token::Type _type;
    std::string _string;
};

#endif // _TOKEN_H
