#include "Parser.h"

#include <iostream>

Parser::Parser(TokenStream& tokenStream) :
  _tokens(tokenStream)
{ /* void */ }

Polygon_set Parser::parsePolygonSetDefinition() {
    while (!_tokens.isDone()) {
        switch(_tokens.peek().type()) {
            case Token::KEYWORD_POLYGON:
                return Polygon_set(parsePolygonDefinition());
            case Token::KEYWORD_MULTIPOLYGON:
                return parseMultipolygonDefinition();
            default:
                std::cerr << "WARNING: Discarded invalid token: " << _tokens.peek().string() << std::endl;
                _tokens.discard();
                break;
        }
    }
    return Polygon_set();
}

Polygon_set Parser::parseMultipolygonDefinition() {
    _tokens.consume(Token::KEYWORD_MULTIPOLYGON);
    if (_tokens.peek().type() == Token::KEYWORD_EMPTY) {
        _tokens.consume(Token::KEYWORD_EMPTY);
        return Polygon_set();
    }
    return parsePolygonSet();
}

Polygon_with_holes Parser::parsePolygonDefinition() {
    _tokens.consume(Token::KEYWORD_POLYGON);
    return parsePolygonWithHoles();
}

Polygon_set Parser::parsePolygonSet() {
    _tokens.consume(Token::LEFT_PARENTHESIS);
    Polygon_set result(parsePolygonWithHoles());
    while (_tokens.peek().type() == Token::COMMA) {
        _tokens.consume(Token::COMMA);
        result.join(parsePolygonWithHoles());
    }
    _tokens.consume(Token::RIGHT_PARENTHESIS);
    return result;
}

Polygon_with_holes Parser::parsePolygonWithHoles() {
    _tokens.consume(Token::LEFT_PARENTHESIS);
    Polygon boundary = parsePolygon();
    if (boundary.orientation() != CGAL::COUNTERCLOCKWISE) {
        std::cerr
            << "WARNING: Polygon boundary is not oriented counter-clockwise. Reversing orientation..."
            << std::endl;
        boundary.reverse_orientation();
    }
    std::list<Polygon> holes;
    while (_tokens.peek().type() == Token::COMMA) {
        _tokens.consume(Token::COMMA);
        Polygon hole = parsePolygon();
        if (hole.orientation() != CGAL::CLOCKWISE) {
            std::cerr
                << "WARNING: Polygon hole is not oriented clockwise. Reversing orientation..."
                << std::endl;
            hole.reverse_orientation();
        }
        holes.push_back(hole);
    }
    _tokens.consume(Token::RIGHT_PARENTHESIS);
    return Polygon_with_holes(boundary, holes.begin(), holes.end());
}

Polygon Parser::parsePolygon() {
    _tokens.consume(Token::LEFT_PARENTHESIS);
    Polygon result;
    result.push_back(parsePoint());
    while (_tokens.peek().type() == Token::COMMA) {
        _tokens.consume(Token::COMMA);
        Point point = parsePoint();
        if (
            _tokens.peek().type() != Token::RIGHT_PARENTHESIS ||
            *result.vertices_begin() != point
        ) {
            result.push_back(point);
        }
    }
    _tokens.consume(Token::RIGHT_PARENTHESIS);
    if (!result.is_simple()) {
        std::cerr << "WARNING: Polygon is not simple." << std::endl;
    }
    return result;
}

Point Parser::parsePoint() {
    double x = _tokens.consume(Token::NUMBER).asDouble();
    double y = _tokens.consume(Token::NUMBER).asDouble();
    return Point(x, y);
}
