// vim: si:ts=4:sw=4:expandtab

#include <vector>
#include <iostream>
#include <CGAL/Cartesian.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/circulator.h>
#include <CGAL/algorithm.h>

//qt
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
typedef CGAL::Creator_uniform_2<double,Point>       Creator;

//the most elegant helper function ever for drawing a convex hull
void drawHull(PointSet hull, QGraphicsScene* scene, QColor color) {
    for(std::vector<Point>::iterator it = hull.begin();
            it != hull.end(); ++it) {
        Point start = *it;
        Point end;
        if((it+1) != hull.end()) {
            end = *(it+1);
        } else {
            end = * hull.begin();
        }
        scene->addLine(
                QLineF(start.x(), -start.y(), end.x(),-end.y()),
                QPen(color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
                );
    }
}


int main(int argc , char* argv[]) {

    PointSet points;
    const int NPOINTS = 42;

    /*
    * Generate a random sample of 42 points for the pointset.
    * Use CGAL's generating tools for this.
    */

    CGAL::Random_points_in_disc_2<Point,Creator> g(200.0);
    CGAL::copy_n(g, NPOINTS, std::back_inserter(points));

    Point q = points.back();
    points.pop_back();

    PointSet hull;
    PointSet hull2;

    //qt setup

    QApplication app(argc, argv);

    QGraphicsScene scene;
    scene.setSceneRect(-400, -400, 800, 800);

    //compute the hull of 41 points
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(hull));

    //draw hull and point
    scene.addEllipse(q.x()-5, -(q.y())-5, 10, 10);
    drawHull(hull, &scene, Qt::green);

    //add one point q to the hull and compute the hull again
    hull.push_back(q);
    CGAL::convex_hull_2(hull.begin(), hull.end(), std::back_inserter(hull2));

    //draw the new hull
    drawHull(hull2, &scene, Qt::red);


    QGraphicsView* view = new QGraphicsView(&scene);
    view->show();

    return app.exec();
}
