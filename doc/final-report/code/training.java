for (File file : files) {

  EntityFile entity = FileReader.read(file);
  
  // compute centroid
  Geometry geometry = entity.getGeometry();
  Point centroid = geometry.getCentroid();
  
  Map<String, String> tags = entity.getTags();
  String populationV = tags.get("population");
  int population = Integer.parseInt(populationV);
  
  // determine place category by population
  String place = "village";
  if (population > 100000) {
    place = "city";
  } else if (population > 10000) {
    place = "town";
  }
  
  // create output line
  writer.append(String.format("%s %f %f %s", id, 
      centroid.getX(), centroid.getY(), place));
  writer.newLine();
}
