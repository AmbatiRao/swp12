Iso_rectangle_2 boundingBox(SiteList sites)
{
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
