#include <iostream>
#include <fstream>
#include <sstream>

#include "CGAL.h"
#include "Parser.h"

std::string readFile(std::string filename) {
    std::ifstream file(filename.c_str());
    if (!file) throw std::exception();
    
    std::string data;
    file.seekg(0, std::ios::end);
    data.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&data[0], data.size());
    
    file.close();
    return(data);
}

std::string pointToWkt(const Point& point) {
    std::ostringstream out;
    out << std::setprecision(10) << point.x() << " " << point.y();
    return out.str();
}

std::string polygonToWkt(const Polygon& polygon) {
    std::ostringstream out;
    out << "(";

    Polygon::Vertex_const_iterator it = polygon.vertices_begin();
    out << pointToWkt(*it);
    ++it;
    while (it != polygon.vertices_end()) {
        out << ", " << pointToWkt(*it);
        ++it;
    }

    out << ")";
    return out.str();
}

std::string polygonWithHolesToWkt(const Polygon_with_holes& polygon) {
    std::ostringstream out;
    
    out << "(" << polygonToWkt(polygon.outer_boundary());
    
    for (
        Polygon_with_holes::Hole_const_iterator it = polygon.holes_begin();
        it != polygon.holes_end();
        ++it
    ) {
        out << ", " << polygonToWkt(*it);
    }
    
    out << ")";
    return out.str();
}

std::string polygonSetToWkt(const Polygon_set& polygonSet) {
    std::ostringstream out;

    std::vector<Polygon_with_holes> polygons;
    polygonSet.polygons_with_holes(std::back_inserter(polygons));

    if (polygons.empty()) {
        out << "MULTIPOLYGON EMPTY";
    } else if (polygons.size() == 1) {
        out << "POLYGON "
            << polygonWithHolesToWkt(polygons[0]);
    } else {
        out << "MULTIPOLYGON (" << polygonWithHolesToWkt(polygons[0]);
        for (unsigned int i = 1; i < polygons.size(); ++i) {
            out << ", " << polygonWithHolesToWkt(polygons[i]);
        }
        out << ")";
    }
    
    out << std::endl;
    return out.str();
}

int main(int argumentCount, char** argumentVector) {
    if (argumentCount < 2) {
        std::cerr
            << "Usage: "
            << argumentVector[0]
            << " <(multi)PolygonFile>, ..."
            << std::endl;
        return 1;
    }
    
    std::cerr << "Loading " << argumentVector[1] << "..." << std::endl;
    std::string polygonString(readFile(argumentVector[1]));
    TokenStream tokenStream(polygonString);
    Parser parser(tokenStream);
    Polygon_set polygon = parser.parsePolygonSetDefinition();
    
    for (int i = 2; i < argumentCount; ++i) {
        std::cerr << "Loading " << argumentVector[i] << "..." << std::endl;
        std::string polygonString2(readFile(argumentVector[i]));
        TokenStream tokenStream2(polygonString2);
        Parser parser2(tokenStream2);
        Polygon_set polygon2 = parser2.parsePolygonSetDefinition();
        polygon.intersection(polygon2);
    }

    std::cout << polygonSetToWkt(polygon);

    return 0;
}
