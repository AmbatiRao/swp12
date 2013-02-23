void handleDual(Segment_2 s, std::vector<PointList>& polylines)
{
        Point_2 ss = s.source();
        Point_2 st = s.target();
        PointList points;
        points.push_back(ss);
        points.push_back(st);
        polylines.push_back(points);
}

void handleDual(Ray_2 r, Iso_rectangle_2 crect, std::vector<PointList>& polylines)
{
        Object_2 o = CGAL::intersection(crect, r);

        Segment_2 seg;
        Point_2 pnt;
        if (assign(seg, o)) {
          Point_2 ss = seg.source();
          Point_2 st = seg.target();
          PointList points;
          points.push_back(ss);
          points.push_back(st);
          polylines.push_back(points);
        } else if (assign(pnt, o)){
          // no use for points
        }
}

void handleDual(Hyperbola_segment_2 hs, std::vector<PointList>& polylines)
{
        PointList p;
        hs.generate_points(p);
        PointList points;
        points.insert(points.end(), p.begin(), p.end());
        polylines.push_back(points);
}

void handleDual(Hyperbola_ray_2 hr, Iso_rectangle_2 crect, std::vector<PointList>& polylines)
{
        PointList p;
        hr.generate_points(p);
        PointList points;
        points.insert(points.end(), p.begin(), p.end());
        polylines.push_back(points);
}
