// standard includes
#include <iostream>
#include <fstream>
#include <cassert>

#include <CGAL/Simple_cartesian.h>

#include <CGAL/Apollonius_graph_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Apollonius_graph_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include "Apollonius_graph_traits_2.h"
//#include <CGAL/Apollonius_graph_traits_2.h>
//#include <CGAL/Apollonius_graph_filtered_traits_2.h>

#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Apollonius_graph_adaptation_traits_2.h>
#include <CGAL/Apollonius_graph_adaptation_policies_2.h>

typedef CGAL::Simple_cartesian<double>                Rep;

//typedef CGAL::Apollonius_graph_filtered_traits_2<Rep> Traits;
typedef CGAL::Apollonius_graph_traits_2<Rep> Traits;
// the second template argument is whether to store the hidden sites.
typedef CGAL::Apollonius_graph_vertex_base_2<Traits,false>  Vb;
typedef CGAL::Triangulation_face_base_2<Traits>       Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>   Agds;
typedef CGAL::Apollonius_graph_2<Traits,Agds>         Apollonius_graph;

typedef Agds::Face                                    Face;
typedef Agds::Face_handle                             Face_handle;
typedef Agds::Vertex                                  Vertex;
typedef Agds::Vertex_handle                           Vertex_handle;

typedef Agds::Face_iterator                           All_faces_iterator;
typedef Agds::Edge_iterator                           All_edges_iterator;
typedef Agds::Vertex_iterator                         All_vertices_iterator;
typedef Apollonius_graph::Finite_faces_iterator       Finite_faces_iterator;
typedef Apollonius_graph::Finite_edges_iterator       Finite_edges_iterator;
typedef Apollonius_graph::Finite_vertices_iterator    Finite_vertices_iterator;
typedef Agds::Face_circulator                         Face_circulator;
typedef Agds::Edge_circulator                         Edge_circulator;

typedef Rep::Iso_rectangle_2                          Iso_rectangle_2;
typedef Traits::Object_2                              Object_2;
typedef Traits::Site_2                                Site_2;
typedef Traits::Point_2                               Point_2;
typedef Traits::Line_2                                Line_2;
typedef Traits::Segment_2                             Segment_2;
typedef Traits::Ray_2                                 Ray_2;
typedef CGAL::Hyperbola_2<Traits>                     Hyperbola_2;
typedef CGAL::Hyperbola_segment_2<Traits>             Hyperbola_segment_2;
typedef CGAL::Hyperbola_ray_2<Traits>                 Hyperbola_ray_2;

typedef std::vector<Site_2>                           SiteList;
typedef std::vector<Point_2>                          PointList;

// forward declarations
SiteList readSites(std::ifstream& ifs, double SF, double wc, double wt, double wv);
SiteList createArtificialSites(SiteList& sites, Iso_rectangle_2 crect);
void printSites(SiteList& sites);

Iso_rectangle_2 boundingBox(SiteList sites);
Iso_rectangle_2 extend(Iso_rectangle_2 rect, double amount);

void handleDual(Object_2 o, Iso_rectangle_2 crect, std::vector<PointList>& polylines);
void handleDual(Line_2 l, Iso_rectangle_2 crect, std::vector<PointList>& polylines);
void handleDual(Segment_2 s, std::vector<PointList>& polylines);
void handleDual(Ray_2 r, Iso_rectangle_2 crect, std::vector<PointList>& polylines);
void handleDual(Hyperbola_segment_2 hs, std::vector<PointList>& polylines);
void handleDual(Hyperbola_ray_2 hr, Iso_rectangle_2 crect, std::vector<PointList>& polylines);
void handleDual(Hyperbola_2 h, Iso_rectangle_2 crect, std::vector<PointList>& polylines);

PointList buildPolygon(Site_2 site, std::vector<PointList>& polylines);
void writeWKT(Site_2 site, PointList polygon, char* outdir);
void writeGeoJSON(Site_2 site, PointList polygon, char* outdir);
void writeSQL(Site_2 site, PointList polygon, char* outdir);
