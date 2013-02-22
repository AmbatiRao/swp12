SiteList readSites(std::ifstream& ifs, double SF, double wc, double wt, double wv)
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
      weight = wc;
    } else if (type == "town") {
      weight = wt;
    } else if (type == "village") {
      weight = wv;
    }
    Site_2 site(Point_2(lon*SF, lat*SF), weight*SF, id);
    sites.push_back(site);
  }
  return sites;
}
