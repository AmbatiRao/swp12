Edge_circulator ecirc = ag.incident_edges(viter), done(ecirc);
std::vector<PointList> polylines;

if (ag.is_infinite(*ecirc)) {
    continue;
}

Object_2 o = ag.dual(*ecirc);
handleDual(o, crect, polylines);
