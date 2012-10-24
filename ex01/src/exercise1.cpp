
#include <CGAL/Cartesian.h>

// include other files here

// Kernel type
typedef CGAL::Cartesian<double> Kernel;

// Point type
typedef Kernel::Point_2 Point_2;

// Segment type
typedef Kernel::Segment_2 Segment_2;

// Signature example of intersection1()
bool intersection1(Segment_2 s1, Segment_2 s2){
    //check for intersection of to segments
    return do_intersect(s1, s2);
}


CGAL::Object intersection2(Segment_2 s1, Segment_2 s2){
    //compte the actual intersection
    Kernel::Intersect_2 intersection;
    return intersection(s1, s2);
}

bool on(Segment_2 s, Point_2 p) {
    //check whether the point lies on the segment
    return do_intersect(s, p);
}

// Put the other two functions here

// Main program
int main( int argc, char* argv[]) {

    //create some points in the plane

    //points for A
    Point_2 a(0, 0), b(2, 0), c(1, 0), d(3, 0);

    //create two segments
    Segment_2 s1(a, b), s2(c, d);

    intersection1(s1, s2);

    std::cout << "Do s1 and s2 intersect? Computer says: " << intersection1(s1, s2) << std::endl;

    CGAL::Object obj = intersection2(s1, s2);

	return 1;
}
