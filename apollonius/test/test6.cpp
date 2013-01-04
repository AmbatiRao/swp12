// vim: si:ts=2:sw=2:expandtab:smartindent
// standard includes
#include <iostream>
#include <fstream>
#include <cassert>

//#include <map>

// example that uses the filtered traits

// choose the representation
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> Rep;

#include <CGAL/Apollonius_graph_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Apollonius_graph_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Apollonius_graph_filtered_traits_2.h>

#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Apollonius_graph_adaptation_traits_2.h>
#include <CGAL/Apollonius_graph_adaptation_policies_2.h>

// Qt includes

#include <CGAL/Qt/Converter.h>
#include <CGAL/Qt/GraphicsViewNavigation.h>
#include <CGAL/Qt/utility.h>
#include <QtGui/QApplication>
#include <QtGui/QTextEdit>
#include <QtGui/QGraphicsView>
#include <QLineF>
#include <QRectF>

// typedef for the traits; the filtered traits class is used
typedef CGAL::Apollonius_graph_filtered_traits_2<Rep> Traits;

// typedefs for the algorithm

// With the second template argument in the vertex base class being
// false, we indicate that there is no need to store the hidden sites.
// One case where this is indeed not needed is when we only do
// insertions, like in the main program below.
typedef CGAL::Apollonius_graph_vertex_base_2<Traits,false>	Vb;
typedef CGAL::Triangulation_face_base_2<Traits>         	Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>		Agds;
typedef CGAL::Apollonius_graph_2<Traits,Agds>    		Apollonius_graph;

typedef Agds::Face 					Face;
typedef Agds::Face_handle				Face_handle;
typedef Agds::Vertex      				Vertex;
typedef Agds::Vertex_handle     			Vertex_handle;

typedef Agds::Face_iterator 				All_faces_iterator;
typedef Agds::Edge_iterator 				All_edges_iterator;
typedef Agds::Vertex_iterator 				All_vertices_iterator;
typedef Apollonius_graph::Finite_faces_iterator		Finite_faces_iterator;
typedef Apollonius_graph::Finite_edges_iterator		Finite_edges_iterator;
typedef Apollonius_graph::Finite_vertices_iterator	Finite_vertices_iterator;
typedef Agds::Face_circulator 				Face_circulator;
typedef Agds::Edge_circulator 				Edge_circulator;

typedef Traits::Object_2				Object_2;
typedef Traits::Site_2					Site_2;
typedef Traits::Point_2					Point_2;
typedef Traits::Line_2					Line_2;
typedef Traits::Segment_2				Segment_2;
typedef Traits::Ray_2      			Ray_2;
typedef Rep::Iso_rectangle_2    Iso_rectangle_2;

typedef CGAL::Apollonius_graph_adaptation_traits_2<Apollonius_graph>			AT;
typedef CGAL::Apollonius_graph_degeneracy_removal_policy_2<Apollonius_graph>		AP;

typedef CGAL::Voronoi_diagram_2<Apollonius_graph,AT,AP>				Voronoi_diagram;
typedef Voronoi_diagram::Face_iterator						VoronoiFace_iterator;
typedef Voronoi_diagram::Face							VoronoiFace;
typedef Voronoi_diagram::Vertex							VoronoiVertex;
typedef Voronoi_diagram::Vertex_handle						VoronoiVertex_handle;
typedef Voronoi_diagram::Halfedge						VoronoiHalfedge;
typedef Voronoi_diagram::Halfedge_handle					VoronoiHalfedge_handle;

int main(int argc , char* argv[])
{
  if (argc != 3) {
    std::cout << "usage: test <input file> <output image>" << std::endl;
    exit(1);
  }

  char* input = argv[1];
  char* output = argv[2];

  std::ifstream ifs(input);
  assert( ifs );

  Apollonius_graph ag;

  std::string line;
  while (std::getline(ifs, line)) {
    std::istringstream iss(line);
    std::string type;
    int id;
    double  lat, lon;
    iss >> id;
    iss >> lon;
    iss >> lat;
    iss >> type;
    Point_2 p(lon, lat);
    Site_2 site(Point_2(lon, lat), 0.01);
    ag.insert(site);

  }

  // validate the Apollonius graph
  assert( ag.is_valid(true, 1) );
  std::cout << std::endl;

  size_t nof = ag.number_of_faces ();
  std::cout << "number of faces: " << nof << std::endl;

  // create the voronoi diagram
  Voronoi_diagram vd(ag);
  for (VoronoiFace_iterator fiter = vd.faces_begin(); fiter != vd.faces_end(); ++fiter) {
    std::cout << "vd face, bounded? " << !fiter->is_unbounded() << std::endl;
  }
  std::cout << "done iterating faces" << std::endl;

  /*
   * visualize with Qt
   */

  QApplication app(argc, argv);
  // Prepare scene
  QGraphicsScene scene;
  //brandenburg von lon 11-15
  //lat von 51-54
  QRectF rect(11, 51, 5, 3);
  scene.setSceneRect(rect); //Has the format: (x,y,width,height)

  // add a circle for each site
  std::cout << "creating circles for sites" << std::endl;
  for (Finite_vertices_iterator viter = ag.finite_vertices_begin();
    viter != ag.finite_vertices_end(); ++viter) {
    Site_2 site = viter->site();
    Point_2 p = site.point();
    double weight = site.weight();
    double size = weight * 2;

    scene.addEllipse(
      p.x() - weight, p.y() - weight, size, size,
      QPen(Qt::red, 3,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
    );
    scene.addEllipse(
      p.x() - 1, p.y() - 1, 2, 2,
      QPen(Qt::red, 1,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
    );
  }

  // add dual image stubs
  std::cout << "dual stubs 1" << std::endl;
  for (All_faces_iterator fiter = ag.all_faces_begin (); fiter != ag.all_faces_end(); ++fiter) { 
    Object_2 o = ag.dual(fiter);
    Site_2 site;
    Line_2 line;
    if (assign(site, o)) {
      Point_2 p = site.point();
      double size = 6;
      scene.addEllipse(
         p.x() - size/2, p.y() - size/2, size, size,
         QPen(Qt::blue, 3,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
       );
    } else if (assign(line, o)) {
    }
  }

  // add dual image stubs
  std::cout << "dual stubs 2" << std::endl;
  for (All_faces_iterator fiter = ag.all_faces_begin (); fiter != ag.all_faces_end(); ++fiter) { 
    Object_2 o = ag.dual(fiter);
    Site_2 site;
    Line_2 line;
    if (assign(site, o)) {
      Point_2 p = site.point();
      double size = 6;
      scene.addEllipse(
         p.x() - size/2, p.y() - size/2, size, size,
         QPen(Qt::blue, 3,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
       );
    } else if (assign(line, o)) {
    }
  }
  std::cout << "number of vertices: " << ag.number_of_vertices() << std::endl;
  std::cout << "vertices iteration 1" << std::endl;
  for (All_vertices_iterator viter = ag.all_vertices_begin (); viter != ag.all_vertices_end(); ++viter) { 
    Face_circulator fcirc = ag.incident_faces(viter), done(fcirc);
    if (fcirc == NULL) {
      break;
    }
    do {
      Object_2 o = ag.dual(fcirc);
    } while(++fcirc != done);
  }

  double lw = 0.001;

  CGAL::Qt::Converter<Rep> convert(rect);
  Iso_rectangle_2 crect = convert(rect);
  std::cout << "rect: " << crect << std::endl;
  for (All_vertices_iterator viter = ag.all_vertices_begin (); viter != ag.all_vertices_end(); ++viter) { 
    Edge_circulator ecirc = ag.incident_edges(viter), done(ecirc);
    do {
      // NOTE: for this to work, we had to make public the dual function in ApolloniusGraph
      // change line 542 in "Apollonius_graph_2.h" from "private:" to "public:"
      if (ag.is_infinite(*ecirc)) {
        // the program may fail in certain situations without this test.
        // acutally !is_infinite(edge) is a precondition in dual(edge).
        continue;
      }
      Object_2 o = ag.dual(*ecirc);
      Line_2 l;
      Segment_2 s;
      Ray_2 r;
      CGAL::Hyperbola_2<Traits> h;
      CGAL::Hyperbola_segment_2<Traits> hs;
      CGAL::Hyperbola_ray_2<Traits> hr;
      if (assign(hs, o)) {
        std::cout << "hyperbola segment" << std::endl; //hs.draw(str);
        std::vector<Point_2> p;
        hs.generate_points(p);
        for (unsigned int i = 0; i < p.size() - 1; i++) {
          Segment_2 seg(p[i], p[i+1]);
          scene.addLine(
            QLineF(p[i].x(), p[i].y(), p[i+1].x(), p[i+1].y()),
            QPen(Qt::green, lw,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }
      }
      else if (assign(s, o)) {
        std::cout << "segment" << std::endl; // str << s; 
        Point_2 ss = s.source();
        Point_2 st = s.target();
        scene.addLine(
          QLineF(ss.x(), ss.y(), st.x(), st.y()),
          QPen(Qt::green, lw,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
      }
      else if (assign(hr, o)) {
        std::cout << "hyperbola ray" << std::endl; // hr.draw(str);
        std::vector<Point_2> p;
        hr.generate_points(p);
        for (unsigned int i = 0; i < p.size() - 1; i++) {
          Segment_2 seg(p[i], p[i+1]);
          scene.addLine(
            QLineF(p[i].x(), p[i].y(), p[i+1].x(), p[i+1].y()),
            QPen(Qt::green, lw,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }
      }
      else if (assign(r, o)) {
        std::cout << "ray" << std::endl;  // str << r;
        Object_2 o = CGAL::intersection(r, crect);

        Segment_2 seg;
        Point_2 pnt;
        if (assign(seg, o)) {
          std::cout << "ray -> segment" << std::endl;
          Point_2 ss = seg.source();
          Point_2 st = seg.target();
          scene.addLine(
            QLineF(ss.x(), ss.y(), st.x(), st.y()),
            QPen(Qt::green, lw,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        } else if (assign(pnt, o)){
          std::cout << "ray -> point" << std::endl;
          // no use for points
        }
      }
      else if (assign(h, o)) {
        std::cout << "hyperbola" << std::endl; // h.draw(str);
        std::vector<Point_2> p, q;
        h.generate_points(p, q);
        for (unsigned int i = 0; i < p.size() - 1; i++) {
          Segment_2 seg(p[i], p[i+1]);
          scene.addLine(
            QLineF(p[i].x(), p[i].y(), p[i+1].x(), p[i+1].y()),
            QPen(Qt::green, lw,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }
        for (unsigned int i = 0; i < q.size() - 1; i++) {
          Segment_2 seg(q[i], q[i+1]);
          scene.addLine(
            QLineF(q[i].x(), q[i].y(), q[i+1].x(), q[i+1].y()),
            QPen(Qt::green, lw,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }
      }
      else if (assign(l, o)) {
        std::cout << "line" << std::endl; //str << l;
        Object_2 o = CGAL::intersection(l, crect);

        Segment_2 seg;
        Point_2 pnt;
        if (assign(seg, o)) {
          std::cout << "line -> segment" << std::endl;
          Point_2 ss = seg.source();
          Point_2 st = seg.target();
          scene.addLine(
            QLineF(ss.x(), ss.y(), st.x(), st.y()),
            QPen(Qt::green, lw,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        } else if (assign(pnt, o)){
          std::cout << "line -> point" << std::endl;
          // no use for points
        }
      }
    } while(++ecirc != done);
  }
  std::cout << "foo" << std::endl;
  // Prepare view, add scene, show
  QGraphicsView* view = new QGraphicsView(&scene);
  //view->show();

  // Add CGAL's navigation filter
  CGAL::Qt::GraphicsViewNavigation navigation;
  view->installEventFilter(&navigation);
  view->viewport()->installEventFilter(&navigation);
  view->setRenderHint(QPainter::Antialiasing);

  // create an image
  // this line creates an image with a size definied by the size of the scene
  //QImage image(scene.sceneRect().size().toSize(), QImage::Format_ARGB32);
  // Cool! Qt does the scaling to arbitrary size automatically. Rocks
  QImage image(QSize(800, 800), QImage::Format_ARGB32);
  image.fill(Qt::transparent);
  QPainter painter(&image);
  painter.setRenderHint(QPainter::Antialiasing);
  scene.render(&painter);
  image.save(output);
 
  //return app.exec();  
}
