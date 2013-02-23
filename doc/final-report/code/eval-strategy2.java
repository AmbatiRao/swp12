    ContainmentTesterFactory containmentTesterFactory = new DiscretizingContainmentTesterFactory(10);
    ContainmentGeometryTesselationMap<String> index = new ContainmentGeometryTesselationMap<String>(
        containmentTesterFactory);

    for (String id : ids) {
      Geometry sourceGeometry = idToSourceGeometry.get(id);
      index.add(sourceGeometry, id);
    }

    GeometryFactory geometryFactory = new GeometryFactory();

    FileInputStream fis = new FileInputStream(filePoints);
    BufferedInputStream is = new BufferedInputStream(fis);

    int i = 0;

    while (true) {
      try {
        double lon = readDouble(is);
        double lat = readDouble(is);

        Point point = geometryFactory.createPoint(new Coordinate(
            lon, lat));

        Set<String> sourceIds = index.test(point);
        if (sourceIds.size() == 0 || sourceIds.size() > 1) {
          continue;
        }
        String id = sourceIds.iterator().next();
        Geometry apolloniusGeometry = idToApolloniusGeometry
            .get(id);

        if (apolloniusGeometry == null) {
          continue;
        }

        boolean inApollonius = apolloniusGeometry.covers(point);
        if (inApollonius) {
          nCorrect += 1;
        }
      } catch (IOException e) {
        break;
      }
    }

    double correct = nCorrect / (double) nTotal;
