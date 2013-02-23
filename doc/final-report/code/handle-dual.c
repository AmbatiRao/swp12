void handleDual(Object_2 o, Iso_rectangle_2 crect, std::vector<PointList>& polylines)
{
  // disambiguation depending on type of o
  Line_2 l;
  Segment_2 s;
  Ray_2 r;
  Hyperbola_2 h;
  Hyperbola_segment_2 hs;
  Hyperbola_ray_2 hr;
  // Hyperbola segment
  if (assign(hs, o)) {
    handleDual(hs, polylines);
  // segment
  } else if (assign(s, o)) {
    handleDual(s, polylines);
  // Hyperbola ray
  } else if (assign(hr, o)) {
    handleDual(hr, crect, polylines);
  // ray
  } else if (assign(r, o)) {
    handleDual(r, crect, polylines);
  // Hyperbola
  } else if (assign(h, o)) {
    handleDual(h, crect, polylines);
  // Line
  } else if (assign(l, o)) {
    handleDual(l, crect, polylines);
  }
}
