// vim: si:ts=2:sw=2:expandtab:smartindent

#include "test.h"

int main(int argc , char* argv[])
{
  if (argc != 3) {
    std::cout << "usage: test <input file> <output folder>" << std::endl;
    exit(1);
  }

  char* input = argv[1];
  char* outdir = argv[2];

  std::ifstream ifs(input);
  assert( ifs );

  /*
   * prepare data
   */

  // we use a ScalingFactor(SF) here to stretch input values at the 
  // beginning, and divide by SF in the end. This is used because the 
  // point-generation of the hyperbola class is using some arbitrary
  // internal decision thresholds to decide how many points to generate for
  // a certain part of the curve. Rule of thumb is: the higher SF the more
  // detail is used in approximation of the hyperbolas.
  double SF = 4000;

  // read in sites from input file
  SiteList sites = readSites(ifs, SF);

  printSites(sites);

  // calculate bounding box of all input sites (and extend it a little).
  // Extension is important, because we later add artificial sites which are
  // actually mirrored on the bounds of this rectangle. If we did not extend
  // some points would lie on the boundary of the bounding box and so would
  // their artificial clones. This would complicate the whole stuff a lot :)
  Iso_rectangle_2 crect = extend(boundingBox(sites), 0.1*SF);
  std::cout << "rect: " << crect << std::endl;

  // a number of artificial sites
  SiteList artificialSites = createArtificialSites(sites, crect);

  /*
   * create Apollonius graph
   */

  Apollonius_graph ag;

  SiteList::iterator itr;
  // add all original sites to the apollonius graph
  for (itr = sites.begin(); itr != sites.end(); ++itr) {
    Site_2 site = *itr;
    ag.insert(site);
  }
  // add all artificial sites to the apollonius graph
  for (itr = artificialSites.begin(); itr != artificialSites.end(); ++itr) {
    Site_2 site = *itr;
    ag.insert(site);
  }

  // validate the Apollonius graph
  assert( ag.is_valid(true, 1) );
  std::cout << std::endl;

  /*
   * create polygons from cells
   */

  // we want an identifier for each vertex within the iteration.
  // this is a loop iteration counter
  int vertexIndex = 0;

  // for each vertex in the apollonius graph (this are the sites)
  for (All_vertices_iterator viter = ag.all_vertices_begin (); 
      viter != ag.all_vertices_end(); ++viter) { 
    // get the corresponding site
    Site_2 site = viter->site();
    Point_2 point = site.point();
    // ignore artifical sites, detect them by their position
    if (!containsPoint(crect, point)) {
      continue;
    }
    std::cout << "vertex " << ++vertexIndex << std::endl;

    // we than circulate all incident edges. By obtaining the respective
    // dual of each edge, we get access to the objects forming the boundary
    // of each voronoi cell in a proper order.
    Edge_circulator ecirc = ag.incident_edges(viter), done(ecirc);
    // this is where we store the polylines
    std::vector<PointList> polylines;
    // for each incident edge
    do {
      // the program may fail in certain situations without this test.
      // acutally !is_infinite(edge) is a precondition in ag.dual(edge).
      if (ag.is_infinite(*ecirc)) {
        continue;
      }
      // NOTE: for this to work, we had to make public the dual function in ApolloniusGraph
      // change line 542 in "Apollonius_graph_2.h" from "private:" to "public:"
      Object_2 o = ag.dual(*ecirc);
      handleDual(o, crect, polylines);
    } while(++ecirc != done);

    PointList polygon = buildPolygon(site, polylines);
    for (int i = 0; i < polygon.size(); i++) {
      Point_2& p = polygon.at(i);
      p = Point_2(p.x()/SF, p.y()/SF);
    }
    writeWKT(site, polygon, outdir);

    // check each point
    for (int i = 0; i < polygon.size(); i++) {
      Point_2 p = polygon.at(i);
      if (p.x() > crect.xmax()/SF || p.x() < crect.xmin()/SF || p.y() > crect.ymax()/SF || p.y() < crect.ymin()/SF) {
        std::cout << "out of bounds" << std::endl;
      }
    }
  }
}

SiteList readSites(std::ifstream& ifs, double SF)
{
  SiteList sites;
  std::string line;
  while (std::getline(ifs, line)) {
    std::istringstream iss(line);
    std::string type;
    long id;
    double  lat, lon;
    iss >> id;
    iss >> lon;
    iss >> lat;
    iss >> type;
    double weight = 0;
    if (type == "city") {
      weight = 0.1;
    } else if (type == "town") {
      weight = 0.04;
    } else if (type == "village") {
      weight = 0.01;
    }
    Site_2 site(Point_2(lon*SF, lat*SF), weight*SF, id);
    sites.push_back(site);
  }
  return sites;
}

SiteList createArtificialSites(SiteList& sites, Iso_rectangle_2 crect)
{
  SiteList artificialSites;
  // add artificial sites
  SiteList::iterator itr;
  for (itr = sites.begin(); itr != sites.end(); ++itr) {
    Site_2 site = *itr;
    Point_2 point = site.point();
    double weight = site.weight();
    Point_2 apoint1(2 * crect.xmin() - point.x(), point.y());
    Site_2 asite1(apoint1, weight);
    Point_2 apoint2(2 * crect.xmax() - point.x(), point.y());
    Site_2 asite2(apoint2, weight);
    Point_2 apoint3(point.x(), 2 * crect.ymin() - point.y());
    Site_2 asite3(apoint3, weight);
    Point_2 apoint4(point.x(), 2 * crect.ymax() - point.y());
    Site_2 asite4(apoint4, weight);
    artificialSites.push_back(asite1);
    artificialSites.push_back(asite2);
    artificialSites.push_back(asite3);
    artificialSites.push_back(asite4);
  }
  return artificialSites;
}

void printSites(SiteList& sites)
{
  SiteList::iterator itr;
  for (itr = sites.begin(); itr != sites.end(); ++itr) {
    Site_2 site = *itr;
    Point_2 point = site.point();
    std::cout << "site: " << site << std::endl;
  }
}

void handleDual(Object_2 o, Iso_rectangle_2 crect, std::vector<PointList>& polylines)
{
  // disambiguation depending on type of o
  Line_2 l;
  Segment_2 s;
  Ray_2 r;
  Hyperbola_2 h;
  Hyperbola_segment_2 hs;
  Hyperbola_ray_2 hr;
  // Hyperbola segment
  if (assign(hs, o)) {
    handleDual(hs, polylines);
  // segment
  } else if (assign(s, o)) {
    handleDual(s, polylines);
  // Hyperbola ray
  } else if (assign(hr, o)) {
    handleDual(hr, crect, polylines);
  // ray
  } else if (assign(r, o)) {
    handleDual(r, crect, polylines);
  // Hyperbola
  } else if (assign(h, o)) {
    handleDual(h, crect, polylines);
  // Line
  } else if (assign(l, o)) {
    handleDual(l, crect, polylines);
  }
}

void handleDual(Line_2 l, Iso_rectangle_2 crect, std::vector<PointList>& polylines)
{
        std::cout << "line" << std::endl; //str << l;
        Object_2 o = CGAL::intersection(l, crect);

        Segment_2 seg;
        Point_2 pnt;
        if (assign(seg, o)) {
          std::cout << "line -> segment" << std::endl;
          Point_2 ss = seg.source();
          Point_2 st = seg.target();
          PointList points;
          points.push_back(ss);
          points.push_back(st);
          polylines.push_back(points);
        } else if (assign(pnt, o)){
          std::cout << "line -> point" << std::endl;
          // no use for points
        }
}

void handleDual(Segment_2 s, std::vector<PointList>& polylines)
{
        Point_2 ss = s.source();
        Point_2 st = s.target();
        std::cout << "segment " << ss << " " << st << std::endl; // str << s; 
        PointList points;
        points.push_back(ss);
        points.push_back(st);
        polylines.push_back(points);
}

void handleDual(Ray_2 r, Iso_rectangle_2 crect, std::vector<PointList>& polylines)
{
        std::cout << "ray" << std::endl;  // str << r;
        Object_2 o = CGAL::intersection(crect, r);

        Segment_2 seg;
        Point_2 pnt;
        if (assign(seg, o)) {
          std::cout << "ray -> segment" << std::endl;
          Point_2 ss = seg.source();
          Point_2 st = seg.target();
          PointList points;
          points.push_back(ss);
          points.push_back(st);
          polylines.push_back(points);
        } else if (assign(pnt, o)){
          std::cout << "ray -> point" << std::endl;
          // no use for points
        } else {
          std::cout << "ray -> ?" << std::endl;
          std::cout << r.source() << " " << r.point(1) << std::endl;
        }
}

void handleDual(Hyperbola_segment_2 hs, std::vector<PointList>& polylines)
{
        std::cout << "hyperbola segment" << std::endl; //hs.draw(str);
        PointList p;
        hs.generate_points(p);
        std::cout << "# hyperbola points: " << p.size() << std::endl;
        PointList points;
        points.insert(points.end(), p.begin(), p.end());
        polylines.push_back(points);
        for (unsigned int i = 0; i < p.size() - 1; i++) {
          Segment_2 seg(p[i], p[i+1]);
          // doing nothing here
        }
}

void handleDual(Hyperbola_ray_2 hr, Iso_rectangle_2 crect, std::vector<PointList>& polylines)
{
        std::cout << "hyperbola ray" << std::endl; // hr.draw(str);
        PointList p;
        hr.generate_points(p);
        PointList points;
        points.insert(points.end(), p.begin(), p.end());
        polylines.push_back(points);
        for (unsigned int i = 0; i < p.size() - 1; i++) {
          Segment_2 seg(p[i], p[i+1]);
          // doing nothing here
        }
}

void handleDual(Hyperbola_2 h, Iso_rectangle_2 crect, std::vector<PointList>& polylines)
{
        std::cout << "hyperbola" << std::endl; // h.draw(str);
        PointList p, q;
        h.generate_points(p, q);
        // TODO: we are not adding anything to points here. This is kind of
        // obsolete since this cannot happen anymore (because of the 
        // artificial sites added in the beginning)
        for (unsigned int i = 0; i < p.size() - 1; i++) {
          Segment_2 seg(p[i], p[i+1]);
          // doing nothing here
        }
        for (unsigned int i = 0; i < q.size() - 1; i++) {
          Segment_2 seg(q[i], q[i+1]);
          // doing nothing here
        }
}

PointList buildPolygon(Site_2 site, std::vector<PointList>& polylines)
{
    // A list for the resulting points.
    PointList points;
    // We build this list from the polylines collected before.
    // One of the problems solved here is that we actually don't know 
    // whether the polylines we get are in order or reversed because of the
    // way we obtained them via Hyperbola_2.generate_points which makes no
    // guarantees about the order of produces points. So we check at each
    // step whether a consecutive polyline has to be added in-order or
    // reversed by inspecting the first and last points respectively. A
    // special case is the first polyline, where we don't have a previous
    // polyline to perform this check for, which is why we make a more 
    // extensive checking when adding the second polyline which also takes
    // care about the first.
    std::cout << "# polylines: " << polylines.size() << std::endl;
    // the first polyline will just be added as it is
    if (polylines.size() > 0) {
      PointList seg = polylines.front();
      points.insert(points.end(), seg.begin(), seg.end());
    }
    // handle consecutive polylines
    if (polylines.size() > 1) {
      // check wheter we can use the second segment this way or whether we
      // have to reverse the first polyline. Use distance comparism to pick
      // the best fit (so that we don't need exact equivalence of endpoints)
      PointList seg = polylines.at(1);
      double d1 = CGAL::squared_distance(points.front(), seg.front());
      double d2 = CGAL::squared_distance(points.front(), seg.back());
      double d3 = CGAL::squared_distance(points.back(), seg.front());
      double d4 = CGAL::squared_distance(points.back(), seg.back());
      // if the first point of the first polyline fits both endpoints of the
      // second polyline better thant the last point of the first polyline,
      // then we reverse the order of the first polyline.
      if ((d1 < d3 && d1 < d4) || (d2 < d3 && d2 < d4)) {
        std::reverse(points.begin(), points.end());
      }
      // for each consecutive polyline
      for (int i = 1; i < polylines.size(); i++) {
        // check which endpoint of this polyline is nearest to the last
        // point in our list of points.
        Point_2 lastPoint = points.back();
        PointList seg = polylines.at(i);
        double d1 = CGAL::squared_distance(lastPoint, seg.front());
        double d2 = CGAL::squared_distance(lastPoint, seg.back());
        if (d1 <= d2) {
          // first point fits better, take polyline in default order
          points.insert(points.end(), ++seg.begin(), seg.end());
        } else {
          // last point fits better, take polyline in reverse order
          points.insert(points.end(), ++seg.rbegin(), seg.rend());
        }
      }
    }
    std::cout << "# points: " << points.size() << std::endl;

    // close polygon if necessary
    if (points.size() > 0){
      Point_2 start = points.front();
      Point_2 end = points.back();
      if (start != end) {
        points.push_back(start);
      }
    }

    if (points.size() > 0 && points.size() < 4) {
      std::cout << "invalid polygon: >0 but <4 points" << std::endl;
      points.clear();
    }

    return points;
}

void writeWKT(Site_2 site, PointList polygon, char* outdir)
{
  // open an output file for storing the WKT
  std::stringstream s;
  s << outdir << "/" << site.id() << ".wkt";
  std::string polygonFileName = s.str();
  std::cout << "filename: " << polygonFileName << std::endl;
  std::ofstream wktFile;
  wktFile.open(polygonFileName.c_str());

  // write WKT file
  wktFile << "POLYGON ((";
  for (int i = 0; i < polygon.size(); i++) {
    Point_2 p = polygon.at(i);
    wktFile << p.x() << " " << p.y();
    if (i < polygon.size() - 1) {
      wktFile << ", ";
    }
  }
  wktFile << "))";
  wktFile.close();
}

/*
 * Test whether the point lies within the specified rectangle
 */
bool containsPoint(Iso_rectangle_2 rect, Point_2 point) {
  return point.x() >= rect.xmin() && point.x() <= rect.xmax()
      && point.y() >= rect.ymin() && point.y() <= rect.ymax();
}

/*
 * calculate the bounding box of all specified sites
 */
Iso_rectangle_2 boundingBox(SiteList sites) {
  double xmin, xmax, ymin, ymax;
  xmin = xmax = sites.front().point().x();
  ymin = ymax = sites.front().point().y();
  SiteList::iterator itr;
  for (itr = sites.begin(); itr != sites.end(); ++itr) {
    Site_2 site = *itr;
    Point_2 point = site.point();
    if (point.x() < xmin) {
      xmin = point.x();
    }
    if (point.x() > xmax) {
      xmax = point.x();
    }
    if (point.y() < ymin) {
      ymin = point.y();
    }
    if (point.y() > ymax) {
      ymax = point.y();
    }
  }
  Iso_rectangle_2 rect(xmin, ymin, xmax, ymax);
  return rect;
}

/*
 * create a rectangle based on the specifed one, but enlarge it by amount
 */
Iso_rectangle_2 extend(Iso_rectangle_2 rect, double amount) {
  Iso_rectangle_2 r(rect.xmin()-amount, rect.ymin()-amount, 
      rect.xmax()+amount, rect.ymax()+amount);
  return r;
}

