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
typedef Kernel::Plane_3                             Plane;
/* Define a container for the point set */
typedef std::vector<Point>                          PointList;
// Each element in this container hold a layer of the onion.
typedef std::vector<PointList>                       Layers; 
typedef CGAL::Creator_uniform_3<double,Point>       Creator;
typedef CGAL::Random                                Random;
typedef CGAL::Oriented_side                         Oriented_side;

typedef CGAL::Polyhedron_3<Kernel>                  Polyhedron;
typedef Polyhedron::Facet                           Facet;
typedef Polyhedron::Facet_iterator                  Facet_iterator;
typedef Polyhedron::Halfedge                        Halfedge;
typedef Polyhedron::Halfedge_handle                 Halfedge_handle;
typedef Polyhedron::Halfedge_iterator               Halfedge_iterator;
typedef Polyhedron::Halfedge_around_facet_circulator Halfedge_facet_circulator;

PointList grabThreeNonlinearPoints(Halfedge_facet_circulator j){
    PointList ps;
    Point p1 = j->vertex()->point();
    ps.push_back(p1);

    Halfedge_facet_circulator it = j;
    it++;
    for (; it != j; it++) {
        Point p = it->vertex()->point();
        if (ps.size() == 1) {
            if (p == ps.at(0)) {
                continue;
            }
            ps.push_back(p);
        } else if (ps.size() == 2) {
            if (collinear(ps.at(0), ps.at(1), p)){
                continue;
            }
            ps.push_back(p);
        }
    }
    return ps;
}

Plane createPlaneFromFacet(Facet f){
        PointList ps = grabThreeNonlinearPoints(f.facet_begin());
        Point p1 = ps.at(0);
        Point p2 = ps.at(1);
        Point p3 = ps.at(2);
        std::cout << "facet: " << p1 << ", " << p2 << ", " << p3 << std::endl;
        Plane plane(p1, p2, p3);
        return plane;
}

int main(int argc, char* argv[]) {
  
    PointList points;
    int npoints;
    std::cout << "Enter the number of points to sample: ";
    std::cin >> npoints;
    std::cout << std::endl;

    if (npoints < 4) {
        exit(1);
    }

    Random random;
  
    // Generate a random sample for computing the initial convex hull
    CGAL::Random_points_in_sphere_3<Point,Creator> g(200.0, random);
    CGAL::copy_n(g, npoints, std::back_inserter(points));

    std::cout << "number of points: " << points.size() << std::endl;

    PointList initPoints;

    /*
     * find four points that are not coplanar
     */
    for(int k = 0; k < points.size(); k++){
        Point p = points.at(k);
        //std::cout << "foo: " << p << std::endl;
        int size = initPoints.size();
        if (size == 0){
            std::cout << "added first point: " << p << std::endl;
            initPoints.push_back(p);
            points.erase(points.begin() + k);
            k--;
        } else if (size == 1) {
            // check if p is not the same as the first one
            if (p == initPoints.at(0)) {
                std::cout << "point is equal to first point: " << p << std::endl;
                continue;
            } else {
                std::cout << "added second point: " << p << std::endl;
                initPoints.push_back(p);
                points.erase(points.begin() + k);
                k--;
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
                points.erase(points.begin() + k);
                k--;
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
                points.erase(points.begin() + k);
                k--;
            }
        }
    }

    std::cout << "number of points remaining: " << points.size() << std::endl;

    /*
     * Initialize the convex hull as a tetrahedon
     */

    // create the inital tetrahedron
    Point p1 = initPoints.at(0);
    Point p2 = initPoints.at(1);
    Point p3 = initPoints.at(2);
    Point p4 = initPoints.at(3);
    Polyhedron P;
    P.make_tetrahedron(p1, p2, p3, p4);

    // add one point that is definitly within the initial tetrahedron
    Vector v1 = p1 - CGAL::ORIGIN;
    Vector v2 = p2 - CGAL::ORIGIN;
    Vector v3 = p3 - CGAL::ORIGIN;
    Vector v4 = p4 - CGAL::ORIGIN;
    Vector v = v1 + v2 + v3 + v4;
    Point pv(v.x() / 4, v.y() / 4, v.z() / 4);
    points.push_back(pv);

    /*
     * Iteratively build the convex hull
     */

    // for each remaining point
    for(std::vector<Point>::iterator it = points.begin(); it != points.end(); ++it) {
        Point p = *it;
        std::cout << std::endl;
        std::cout << "point: " << p << std::endl;
        // traverse all facets
        bool inside = true;
        for (Facet_iterator it = P.facets_begin(); it != P.facets_end(); ++it) {
            Facet f = *it;
            // create plane from facet
            Plane plane = createPlaneFromFacet(f);
            // check orientation of point towards plane
            Oriented_side o = plane.oriented_side(p);
            bool visible = o == CGAL::NEGATIVE;
            std::cout << "orientation: " << o << ", " << (visible ? "visible" : "invisible") << std::endl;
            if (visible) {
                inside = false;
                std::cout << "removing facet" << std::endl;
                Halfedge_handle halfedge = f.halfedge();
                P.erase_facet(halfedge);
            }
        }
        if (inside){
            continue;
        }

        // find first border halfedge
        Halfedge_handle borderStart;
        bool foundBorder = false;
        // NOTE: could also iterate edges instead of halfedges, may be faster
        for (Halfedge_iterator it = P.edges_begin(); it != P.edges_end(); it++){
            Halfedge halfedge = *it;
            if (halfedge.is_border_edge()){
                if (halfedge.is_border()){
                    borderStart = it;
                }else{
                    borderStart = halfedge.opposite();
                }
                foundBorder = true;
                break;
            }
        }
        if (!foundBorder){
            continue;
        }
        std::cout << "found border" << std::endl;

        // iterate horizont
        Halfedge_handle it = borderStart; 
        do {
            std::cout << "border halfedge" << std::endl;
            // TODO: collect vertices
            
            it = it -> next();
        } while(it != borderStart);
    }

}
