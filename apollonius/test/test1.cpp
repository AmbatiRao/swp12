// standard includes
#include <iostream>
#include <fstream>
#include <cassert>

// the number type
#include <CGAL/MP_Float.h>


// example that uses an exact number type

typedef CGAL::MP_Float NT;

// choose the kernel
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<NT>  Kernel;

// typedefs for the traits and the algorithm

#include <CGAL/Apollonius_graph_2.h>
#include <CGAL/Apollonius_graph_traits_2.h>

typedef CGAL::Apollonius_graph_traits_2<Kernel>   Traits;
typedef CGAL::Apollonius_graph_2<Traits>          Apollonius_graph;

typedef Apollonius_graph::Face_iterator All_faces_iterator;

int main()
{
  std::ifstream ifs("../data/sites.cin");
  assert( ifs );

  Apollonius_graph ag;
  Apollonius_graph::Site_2 site;

  int k = 0;
  // read the sites and insert them in the Apollonius graph
  while ( ifs >> site ) {
    std::cout << "inserting: " << ++k << std::endl;
    std::cout << site << std::endl;
    ag.insert(site);
    size_t nof = ag.number_of_faces ();
    std::cout << "number of faces: " << nof << std::endl;
  }

  // validate the Apollonius graph
  assert( ag.is_valid(true, 1) );
  std::cout << std::endl;

  size_t nof = ag.number_of_faces ();
  std::cout << "number of faces: " << nof << std::endl;

  //All_faces_iterator fiter = ag.all_faces_begin ();

  return 0;
}
