// vim: si:ts=4:sw=4:expandtab

#include <vector>
#include <iostream>
#include <fstream>
#include <CGAL/Simple_cartesian.h>
//#include <CGAL/Cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/circulator.h>
#include <CGAL/algorithm.h>
#include <CGAL/Random.h>

//#include <CGAL/Qt/GraphicsViewNavigation.h>
//#include <QtGui/QApplication>
//#include <QtGui/QTextEdit>
//#include <QtGui/QGraphicsView>
//#include <QLineF>

#include <sys/time.h>

//typedef CGAL::Cartesian<double>                     Kernel;
typedef CGAL::Simple_cartesian<double>              Kernel;
typedef Kernel::Point_3                             Point;
//typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;
typedef Kernel::Vector_3                            Vector;
/* Define a container for the point set */
typedef std::vector<Point>                          PointSet;
// Each element in this container hold a layer of the onion.
typedef std::vector<PointSet>                       Layers; 
typedef CGAL::Creator_uniform_3<double,Point>       Creator;
typedef CGAL::Random                                Random;

typedef CGAL::Polyhedron_3<Kernel>                  Polyhedron;
typedef Polyhedron::Facet                           Facet;
typedef Polyhedron::Facet_iterator                  Facet_iterator;

int main(int argc, char* argv[]) {
  
    PointSet points;
    int npoints;
    std::cout << "Enter the number of points to sample: ";
    std::cin >> npoints;
    std::cout << std::endl;

    Random random;
  
    // Generate a random sample for computing the initial convex hull
    CGAL::Random_points_in_sphere_3<Point,Creator> g(200.0, random);
    CGAL::copy_n(g, npoints, std::back_inserter(points));

    PointSet initPoints;

    /*
     * find four points that are not coplanar
     */
    for(std::vector<Point>::iterator it = points.begin(); it != points.end(); ++it) {
        Point p = *it;
        //std::cout << "foo: " << p << std::endl;
        int size = initPoints.size();
        if (size == 0){
            initPoints.push_back(p);
            std::cout << "added first point: " << p << std::endl;
        } else if (size == 1) {
            // check if p is not the same as the first one
            if (p == initPoints.at(0)) {
                std::cout << "point is equal to first point: " << p << std::endl;
                continue;
            } else {
                std::cout << "added second point: " << p << std::endl;
                initPoints.push_back(p);
            }
        } else if (size == 2) {
            // check if p is not colinear with the first two
            Point a = initPoints.at(0);
            Point b = initPoints.at(1);
            if (CGAL::collinear(a, b, p)){
                std::cout << "point is collinear with first two points: " << p << std::endl;
                continue;
            } else {
                std::cout << "added third point: " << p << std::endl;
                initPoints.push_back(p);
            }
        } else if (size == 3) {
            // check if p is not coplanar with the first three
            Point a = initPoints.at(0);
            Point b = initPoints.at(1);
            Point c = initPoints.at(2);
            if (CGAL::coplanar(a, b, c, p)){
                std::cout << "point is coplanar with first three points: " << p << std::endl;
                continue;
            } else {
                std::cout << "added fourth point: " << p << std::endl;
                initPoints.push_back(p);
            }
        }
    }

    /*
     * Initialize the convex hull as a tetrahedon
     */

    Point p1 = initPoints.at(0);
    Point p2 = initPoints.at(1);
    Point p3 = initPoints.at(2);
    Point p4 = initPoints.at(3);
    Polyhedron P;
    P.make_tetrahedron(p1, p2, p3, p4);

    /*
     * Iteratively build the convex hull
     */

     for (Facet_iterator it = P.facets_begin(); it != P.facets_end(); ++it) {
         Facet f = *it;
         //std::cout << "facet: " << f << std::endl;
     }
}
