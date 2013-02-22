void writeGeoJSON(Site_2 site, PointList polygon, char* outdir)
{
  // open an output file for storing the GeoJSON
  std::stringstream s;
  s << outdir << "/" << site.id() << ".geojson";
  std::string polygonFileName = s.str();
  std::cout << "filename: " << polygonFileName << std::endl;
  std::ofstream file;
  file.open(polygonFileName.c_str());

  // write data
  file << "{\"type\":\"Polygon\",\"coordinates\":[[";
  for (int i = 0; i < polygon.size(); i++) {
    Point_2 p = polygon.at(i);
    file << "[" << p.x() << ", " << p.y() << "]";
    if (i < polygon.size() - 1) {
      file << ", ";
    }
  }
  file << "]]}";
  file.close();
}
