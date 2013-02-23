    // this value is actually read from command line
    int numPoints = 1000000;

    /*
     * calculate area for each source polygon and store it in a map for
     * later retrieval. while at hand, also calculate the total area.
     */
    Map<String, Double> areas = new HashMap<String, Double>();
    double totalArea = 0;

    for (String id : ids) {
      Geometry geometry = idToSourceGeometry.get(id);
      double area = geometry.getArea();
      areas.put(id, area);
      totalArea += area;
    }

    int nCorrect = 0;

    /*
     * for each source geometry, calculate some test points
     */
    Random random = new Random();
    GeometryFactory geometryFactory = new GeometryFactory();
    for (String id : ids) {
      Geometry sourceGeometry = idToSourceGeometry.get(id);
      Geometry apolloniusGeometry = idToApolloniusGeometry.get(id);

      double area = areas.get(id);
      int nPoints = (int) Math.ceil((area / totalArea) * numPoints);

      if (apolloniusGeometry == null) {
        nWrong += nPoints;
        continue;
      }

      ContainmentTester ctSource = new DiscretizingContainmentTester(
          sourceGeometry, 10);
      ContainmentTester ctApollonius = new DiscretizingContainmentTester(
          apolloniusGeometry, 10);

      Envelope bbox = sourceGeometry.getEnvelopeInternal();
      for (int i = 0; i < nPoints; i++) {
        Point point = generatePointWithin(ctSource, bbox, random,
            geometryFactory);
        boolean inApollonius = ctApollonius.covers(point);
        if (inApollonius) {
          nCorrect += 1;
        }
      }
    }

    double correct = nCorrect / (double) nTotal;
