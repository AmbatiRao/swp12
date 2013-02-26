void writeSQL(Site_2 site, PointList polygon, char* outdir)
{
  std::cout << "INSERT INTO " << outdir << " (geom, id) VALUES (";
  std::cout << "'POLYGON ((";
  for (int i = 0; i < polygon.size(); i++) {
    Point_2 p = polygon.at(i);
    std::cout << p.x() << " " << p.y();
    if (i < polygon.size() - 1) {
      std::cout << ", ";
    }
  }
  std::cout << "))', " << site.id() << ");" << std::endl;
}
