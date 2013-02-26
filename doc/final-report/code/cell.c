  // for each vertex in the apollonius graph (this are the sites)
  for (All_vertices_iterator viter = ag.all_vertices_begin (); 
      viter != ag.all_vertices_end(); ++viter) { 
    // get the corresponding site
    Site_2 site = viter->site();
    Point_2 point = site.point();
    // ignore artifical sites, detect them by their position
    if (!CGAL::do_intersect(crect, point)) {
      continue;
    }

    // we than circulate all incident edges. By obtaining the respective
    // dual of each edge, we get access to the objects forming the boundary
    // of each voronoi cell in a proper order.
    Edge_circulator ecirc = ag.incident_edges(viter), done(ecirc);
    // this is where we store the polylines
    std::vector<PointList> polylines;
    // for each incident edge
    do {
      // the program may fail in certain situations without this test.
      // acutally !is_infinite(edge) is a precondition in ag.dual(edge).
      if (ag.is_infinite(*ecirc)) {
        continue;
      }
      // NOTE: for this to work, we had to make public the dual function in ApolloniusGraph
      // change line 542 in "Apollonius_graph_2.h" from "private:" to "public:"
      Object_2 o = ag.dual(*ecirc);
      handleDual(o, crect, polylines);
    } while(++ecirc != done);

    PointList polygon = buildPolygon(site, polylines);

    for (int i = 0; i < polygon.size(); i++) {
      Point_2& p = polygon.at(i);
      p = Point_2(p.x()/SF, p.y()/SF);
    }

    [...]

  }
