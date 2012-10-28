// vim: si:ts=4:sw=4:expandtab
/*
 * Compute and visualize the convex layers of a point set in the plane.
 *
 * Author: Dror Atariah <dror.atariah@fu-berlin.de>, Oct. 2012
 *
 * An optimal algorithm is given by B. Chazelle
 * "On the Convex Layers of a Planar Set" in
 * https://www.cs.princeton.edu/~chazelle/pubs/ConvexLayers.pdf
 */

#include <vector>
#include <iostream>
#include <CGAL/Cartesian.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/circulator.h>
#include <CGAL/algorithm.h>

#include <CGAL/Qt/GraphicsViewNavigation.h>
#include <QtGui/QApplication>
#include <QtGui/QTextEdit>
#include <QtGui/QGraphicsView>
#include <QLineF>

typedef CGAL::Cartesian<double>                     Kernel;
typedef Kernel::Point_2                             Point;
/* Define a container for the point set */
typedef std::vector<Point>                          PointSet;
// Each element in this container hold a layer of the onion.
typedef std::vector<PointSet>                       Layers; 
typedef CGAL::Creator_uniform_2<double,Point>       Creator;

int main(int argc , char* argv[]) {
  
    PointSet points;
    int NPOINTS;
    std::cout << "Enter the number of points to sample: ";
    std::cin >> NPOINTS;
  
    /*
    * Generate a random sample for the pointset.
    * Use CGAL's generating tools for this!
    */

    CGAL::Random_points_in_disc_2<Point,Creator> g(200.0);
    CGAL::copy_n(g, NPOINTS, std::back_inserter(points));

    /*
    * Compute the convex layers of the point set
    */

    Layers layers;

    while(points.size() >= 3) {
        std::vector<Point> hull;
        CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(hull));
        layers.push_back(hull);

        //std::cout << "#points: " << points.size() << std::endl;
        // for each hull points
        for(std::vector<Point>::iterator it = hull.begin(); it != hull.end(); ++it) {
            Point i = *it;
            std::vector<Point>::iterator it = find (points.begin(), points.end(), i);
            if (it == points.end()) {
                // point could not be found, should not happen
                continue;
            }
            //std::cout << "remove: " << *it << std::endl;
            points.erase(it);
        }
        //std::cout << "#points: " << points.size() << std::endl;
    }

    /*
     * show with QT
     */

    QApplication app(argc, argv);
  
    // Prepare scene
    QGraphicsScene scene;
    scene.setSceneRect(-400, -400, 800, 800); //Has the format: (x,y,width,height)
  
    // Draw points
    for(std::vector<Point>::iterator it = points.begin(); it != points.end(); ++it) {
        Point i = *it;
        scene.addEllipse(i.x()-5, -(i.y())-5, 10, 10);
    }

    // Draw the convex layers
    for(std::vector<PointSet>::iterator it = layers.begin(); it != layers.end(); ++it) {
        PointSet layer = *it;
        // use CGAL circulator adaptor for the convex hull
        CGAL::Const_circulator_from_container<PointSet> 
            c1(&layer), c2(&layer), c3(&layer);
        if (c1 != 0){
            do {
                ++c2;
                scene.addLine(
                    QLineF((*c1).x(), -(*c1).y(), (*c2).x(), -(*c2).y()),
                        QPen(Qt::green, 3,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
                );
                scene.addEllipse((*c1).x()-5, -((*c1).y())-5, 10, 10);
            } while (++c1 != c3);    
        }
    }

    // Prepare view, add scene, show
    QGraphicsView* view = new QGraphicsView(&scene);
    view->show();

    // Add CGAL's navigation filter
    CGAL::Qt::GraphicsViewNavigation navigation;
    view->installEventFilter(&navigation);
    view->viewport()->installEventFilter(&navigation);
    view->setRenderHint(QPainter::Antialiasing);
  
    return app.exec();  
}
