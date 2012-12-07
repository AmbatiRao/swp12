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

// Qt includes

#include <CGAL/Qt/GraphicsViewNavigation.h>
#include <QtGui/QApplication>
#include <QtGui/QTextEdit>
#include <QtGui/QGraphicsView>
#include <QLineF>

// typedef for the traits; the filtered traits class is used
typedef CGAL::Apollonius_graph_filtered_traits_2<Rep> Traits;

// typedefs for the algorithm

// With the second template argument in the vertex base class being
// false, we indicate that there is no need to store the hidden sites.
// One case where this is indeed not needed is when we only do
// insertions, like in the main program below.
typedef CGAL::Apollonius_graph_vertex_base_2<Traits,false>   Vb;
typedef CGAL::Triangulation_face_base_2<Traits>              Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>       Agds;
typedef CGAL::Apollonius_graph_2<Traits,Agds>    Apollonius_graph;

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

typedef Traits::Object_2				Object_2;
typedef Traits::Site_2					Site_2;
typedef Traits::Point_2					Point_2;
typedef Traits::Line_2					Line_2;

int main(int argc , char* argv[])
{
  std::ifstream ifs("../data/sites.cin");
  assert( ifs );

  Apollonius_graph ag;
  Apollonius_graph::Site_2 site;

  int k = 0;
  // read the sites and insert them in the Apollonius graph
  while ( ifs >> site ) {
    std::cout << "inserting: " << ++k << ": " << site << std::endl;
    std::cout << "position: " << site.point() << ", weight: " << site.weight() << std::endl;
    ag.insert(site);
    size_t nof = ag.number_of_faces ();
    std::cout << "number of faces: " << nof << std::endl;
  }

  // validate the Apollonius graph
  assert( ag.is_valid(true, 1) );
  std::cout << std::endl;

  size_t nof = ag.number_of_faces ();
  std::cout << "number of faces: " << nof << std::endl;

  std::map<Vertex_handle,int> V;
  Vertex_handle infinite_vertex;
  int inum = 0;
  V[infinite_vertex] = inum++;
  for (Finite_vertices_iterator viter = ag.finite_vertices_begin();
       viter != ag.finite_vertices_end(); ++viter) {
       V[viter] = inum++;
       std::cout << "vertex " << inum << ": " << viter->site() << std::endl;
       // viter also has access to the hidden sites via hidden_sites_begin()...
  }

  inum = 0;
  for (All_faces_iterator fiter = ag.all_faces_begin (); fiter != ag.all_faces_end(); ++fiter) { 
    std::cout << "face " << inum++ << ": ";
    for (int j = 0; j < 3; j++) {
      std::cout << V[ fiter->vertex(j) ] << " ";
      // fiter also has acces to the facet-neighbors via neighbor()...
    }
    std::cout << std::endl;
  }
  
  inum = 0;
  for (All_faces_iterator fiter = ag.all_faces_begin (); fiter != ag.all_faces_end(); ++fiter) { 
    std::cout << "face " << inum++ << ": ";

    Object_2 o = ag.dual(fiter);
    Site_2 site;
    Line_2 line;
    if (assign(site, o)) {
      std::cout << "site";
    } else if (assign(line, o)) {
      std::cout << "line";
    }
    std::cout << std::endl;
  }

  /*
   * for each site in the apo graph: get the surrounding faces. for each of
   * those faces, get the dual, i.e. site/line. connecting the sites should
   * form the cell boundary.
   */

  inum = 0;
  for (All_edges_iterator eiter = ag.all_edges_begin (); eiter != ag.all_edges_end(); ++eiter) { 
    //std::cout << "edge " << inum++ << ": ";
    // ag.dual(*eiter); // Die ist leider private
    //std::cout << std::endl;
  }

//  for (All_faces_iterator fiter = ag.all_faces_begin (); fiter != ag.all_faces_end(); ++fiter) { 
//    Face face = *fiter;
//    Vertex_handle vertex0 = face.vertex(0);
//    Vertex_handle vertex1 = face.vertex(1);
//    Vertex_handle vertex2 = face.vertex(2);
//    Vertex v0 = *vertex0;
//    Vertex v1 = *vertex1;
//    Vertex v2 = *vertex2;
//    std::cout << "vertices: " << v0 << v1 << v2 << std::endl;
//    //std::cout << (fiter->neighbor(0)) << std::endl;
//    //v.point();
//    //std::cout << "face: " << face << std::endl;
//    //std::cout << "vertex: " << vertex.point() << std::endl;
//    //std::cout << "vertex: " << v << std::endl;
//  }
//
//  for (Finite_faces_iterator fiter = ag.finite_faces_begin (); fiter != ag.finite_faces_end(); ++fiter) { 
//    Face face = *fiter;
//    std::cout << "face: " << face << std::endl;
//  }

  std::cout << "file_output:" << std::endl;
  ag.file_output(std::cout);

  /*
   * visualize with Qt
   */

  QApplication app(argc, argv);
  // Prepare scene
  QGraphicsScene scene;
  scene.setSceneRect(-400, -400, 800, 800); //Has the format: (x,y,width,height)

  scene.addLine(
    QLineF(100, 100, 200, 300),
    QPen(Qt::green, 3,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
  );

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
  }

  // Prepare view, add scene, show
  QGraphicsView* view = new QGraphicsView(&scene);
  //view->show();

  // Add CGAL's navigation filter
  CGAL::Qt::GraphicsViewNavigation navigation;
  view->installEventFilter(&navigation);
  view->viewport()->installEventFilter(&navigation);
  view->setRenderHint(QPainter::Antialiasing);

  // create an image
  QImage image(scene.sceneRect().size().toSize(), QImage::Format_ARGB32);
  image.fill(Qt::transparent);
  QPainter painter(&image);
  painter.setRenderHint(QPainter::Antialiasing);
  scene.render(&painter);
  image.save("image.png");
 
  //return app.exec();  
}
