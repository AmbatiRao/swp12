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
