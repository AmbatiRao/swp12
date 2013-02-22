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

    return points;
}
