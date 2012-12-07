#include "typedefs.h"
int main() {
    std::list<Point> points;

    Triangulation triangulation(points.begin(), points.end());

    for(int x=0; x < 10; x++) {
        for(int y=0; y < 10; y++) {
            for(int z=0; z < 10; z++) {
                triangulation.insert(Point(x,y,z));
            }
        }
    }

    std::cout << "Number of vertices: " << triangulation.number_of_vertices()
        << std::endl;

}
