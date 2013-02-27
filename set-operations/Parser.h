#ifndef _PARSER_H
#define _PARSER_H

#include "TokenStream.h"
#include "CGAL.h"

#include <list>
#include <vector>

class Parser {
public:
    Parser(TokenStream& tokenStream);

    Polygon_set parsePolygonSetDefinition();
    Polygon_set parseMultipolygonDefinition();
    Polygon_with_holes parsePolygonDefinition();
    Polygon_set parsePolygonSet();
    Polygon_with_holes parsePolygonWithHoles();
    Polygon parsePolygon();
    Point parsePoint();

private:    
    TokenStream& _tokens;
};

#endif // _PARSER_H
