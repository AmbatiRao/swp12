// vim: si:ts=4:sw=4:expandtab
/*
 * Performance evaluation of different convex hull algorithm from CGAL
 */

#include <vector>
#include <iostream>
#include <CGAL/Cartesian.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/circulator.h>
#include <CGAL/algorithm.h>
#include <CGAL/Random.h>
#include <CGAL/Polygon_2.h>

#include <CGAL/Qt/GraphicsViewNavigation.h>
#include <QtGui/QApplication>
#include <QtGui/QTextEdit>
#include <QtGui/QGraphicsView>
#include <QLineF>

typedef CGAL::Cartesian<double>                     Kernel;
//typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;
typedef Kernel::Point_2                             Point;
typedef Kernel::Vector_2                            Vector;
/* Define a container for the point set */
typedef std::vector<Point>                          PointSet;
// Each element in this container hold a layer of the onion.
typedef std::vector<PointSet>                       Layers; 
typedef CGAL::Creator_uniform_2<double,Point>       Creator;
typedef CGAL::Random                                Random;
typedef CGAL::Polygon_2<Kernel>                     Polygon;
typedef CGAL::Bbox_2                                Bbox;

void go(std::vector<Point> hull, int NPOINTS, int config);

int main(int argc, char* argv[]) {
  
    PointSet points;
    int NPOINTS;
    std::cout << "Enter the number of points to sample: ";
    std::cin >> NPOINTS;

    // Number of points to generate to compute the convex hull, which in turn
    // will be used for generating points.
    int INITPOINTS = 100;
  
    // Generate a random sample for computing the convex hull
    CGAL::Random_points_in_disc_2<Point,Creator> g(200.0);
    CGAL::copy_n(g, INITPOINTS, std::back_inserter(points));

    // the convex hull used for point generation
    std::vector<Point> hull;
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(hull));

    QApplication app(argc, argv);

    for (int config = 0; config <= 4; config++) {
        go(hull, NPOINTS, config);
    }

    return 0;
}

void go(std::vector<Point> hull, int NPOINTS, int config) {
    /*
     * compute points on the boundary and within the convex hull
     */

    int nOn = (int) (config / 4.0 * NPOINTS);
    int nIn = NPOINTS - nOn;

    std::vector<Point> onHull;
    std::vector<Point> inHull;

    Random random;
    int vertices = hull.size();

    /*
     * Another approach to generate points on the boundary of the convex hull.
     * This algorithm picks a random segment and then picks a random points
     * within this segment. This penalizes long segments.
     */

//    for (int i = 0; i < nOn; i++) {
//        int v = random.get_int(0, vertices);
//        double c = random.get_double(0.0, 1.0);
//        Point p1 = hull.at(v);
//        Point p2 = hull.at((v + 1) % vertices);
//        Vector d = p2 - p1;
//        Point p = p1 + d * c;
//        onHull.push_back(p);
//    }

    /*
     * Compute random points on the boundary of the convex hull.
     * Do this by first calculating the length of the boundary 'sum' and
     * then selecting 'c' randomly from the interval [0, sum). This c
     * denotes a position on the boundary of the convex hull, which we take
     * as our sample point
     */

    double sum = 0; // length of the boundary
    double lengths[vertices]; // lengths of the segments
    double positions[vertices]; // starting positions of the segments
    for (int i = 0; i < vertices; i++) {
        Point p1 = hull.at(i);
        Point p2 = hull.at((i + 1) % vertices);
        Vector d = p2 - p1;
        lengths[i] = sqrt(d.squared_length());
        sum += lengths[i];
        positions[i] = sum;
    }

    // generate the points
    for (int i = 0; i < nOn; i++) {
        double c = random.get_double(0.0, sum); // total position on the hull
        int k = 0; // index of the first interpolation point p1
        double before = 0; // cumulated sum of lengths to reach p1 from start
        while(positions[k] < c) {
            before = positions[k];
            k++;
        }
        double rem = c - before; // position on the k'th segment
        // interpolate between p1 and p2
        Point p1 = hull.at(k);
        Point p2 = hull.at((k + 1) % vertices);
        Vector d = p2 - p1;
        Point p = p1 + d * (rem / lengths[k]);
        onHull.push_back(p);
    }

    /*
     * One approach for generating points within the convex hull.
     * The algorithm generates a convex combination of all vertices forming
     * the convex hull polygon. It does so by generating a weight between 0
     * and 1 for each vertex. This weight is normalized with the total weight
     * generated and the weight of each vertex is this normalized weight.
     * This approach leads to points distributed in a small radius within
     * the center of the convex hull.
     */

//    for (int i = 0; i < nIn; i++) {
//        double values[vertices];
//        double sum = 0;
//        for (int k = 0; k < vertices; k++) {
//            values[k] = random.get_double(0.0, 1.0);
//            sum += values[k];
//        }
//        Vector v(0,0);
//        for (int k = 0; k < vertices; k++) {
//            v = v + (hull.at(k) - CGAL::ORIGIN) * values[k] / sum;
//        }
//        Point p = CGAL::ORIGIN + v;
//        std::cout << "p:  " << p << std::endl;
//        inHull.push_back(p);
//    }

    /*
     * Another approach for generating points within the convex hull.
     * The algorithm randomly selects points within the bounding box and 
     * adds them to the list of selected points if they fall into the convex
     * hull.
     */

    Polygon poly(hull.begin(), hull.end());
    Bbox bbox = poly.bbox();
    while (inHull.size() < nIn) {
        double x = random.get_double(bbox.xmin(), bbox.xmax());
        double y = random.get_double(bbox.ymin(), bbox.ymax());
        Point p(x, y);
        if (poly.bounded_side(p) == CGAL::ON_BOUNDED_SIDE) {
            inHull.push_back(p);
        }
    }

    /*
     * show with QT
     */
  
    // Prepare scene
    QGraphicsScene scene;
    scene.setSceneRect(-250, -250, 500, 500);
  
    // Draw points
    for(std::vector<Point>::iterator it = onHull.begin(); it != onHull.end(); ++it) {
        Point i = *it;
        scene.addEllipse(i.x()-5, -(i.y())-5, 10, 10,
            QPen(Qt::black, 1,  Qt::SolidLine));
    }

    for(std::vector<Point>::iterator it = inHull.begin(); it != inHull.end(); ++it) {
        Point i = *it;
        scene.addEllipse(i.x()-5, -(i.y())-5, 10, 10,
            QPen(Qt::blue, 1,  Qt::SolidLine));
    }

    // Draw the convex layers
    PointSet layer = hull;
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
            scene.addEllipse((*c1).x()-5, -((*c1).y())-5, 10, 10,
                QPen(Qt::red, 1,  Qt::SolidLine));
        } while (++c1 != c3);    
    }

    /*
     * create an image
     */
    char buffer[100];
    sprintf(buffer, "image%d.png", config);

    QImage image(scene.sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    scene.render(&painter);
    image.save(buffer);

    /*
     * show stuff in QT window
     */

    // Prepare view, add scene, show
//    QGraphicsView* view = new QGraphicsView(&scene);
//    view->show();

    // Add CGAL's navigation filter
//    CGAL::Qt::GraphicsViewNavigation navigation;
//    view->installEventFilter(&navigation);
//    view->viewport()->installEventFilter(&navigation);
//    view->setRenderHint(QPainter::Antialiasing);

    //return app.exec();  

}
