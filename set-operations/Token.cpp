#include "Token.h"

#include <boost/lexical_cast.hpp>

/* static */ std::string Token::typeString(Token::Type tokenType) {
    switch (tokenType) {
        case NUMBER:
            return std::string("<number>");
            break;
        case LEFT_PARENTHESIS:
            return std::string("(");
            break;
        case RIGHT_PARENTHESIS:
            return std::string(")");
            break;
        case COMMA:
            return std::string(",");
            break;
        case KEYWORD_POLYGON:
            return std::string("POLYGON");
            break;
        case KEYWORD_MULTIPOLYGON:
            return std::string("MULTIPOLYGON");
            break;
        case KEYWORD_EMPTY:
            return std::string("EMPTY");
            break;
        case UNKNOWN: default:
            return std::string("<unknown>");
            break;
    }
}

Token::Token(Token::Type type, std::string string) :
    _type(type),
    _string(string)
{ /* void */ }

Token::Type Token::type() const { return _type; }

std::string Token::typeString() const { return typeString(type()); }

std::string Token::string() const { return _string; }

double Token::asDouble() const {
    return boost::lexical_cast<double>(string());
}