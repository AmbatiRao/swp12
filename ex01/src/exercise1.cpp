
#include <CGAL/Cartesian.h>

/*
 * some convenience functions for printing
 */

void echo (const char * text){
     std::cout << text << std::endl;
}

void newline (){
     std::cout << std::endl;
}

/*
 * CGAL boilerplate code
 */

// Kernel type
typedef CGAL::Cartesian<double> Kernel;

// Point type
typedef Kernel::Point_2 Point_2;

// Segment type
typedef Kernel::Segment_2 Segment_2;

/*
 * geometric functions
 */

// test whether two segments intersect
bool intersection1(Segment_2 s1, Segment_2 s2){
    //check for intersection of to segments
    return do_intersect(s1, s2);
}

// compute the intersection of two segments
CGAL::Object intersection2(Segment_2 s1, Segment_2 s2){
    //compte the actual intersection
    Kernel::Intersect_2 intersection;
    return intersection(s1, s2);
}

// test whether a point lies on a segment
bool on(Segment_2 s, Point_2 p) {
    //check whether the point lies on the segment
    return do_intersect(s, p);
}

/*
 * helper functions that calculate something and then print the results
 */

void printSegments(Segment_2 s1, Segment_2 s2){
    std::cout << "s1: " << s1 << " s2: " << s2 << std::endl;
}

void testForIntersectionAndPrint(Segment_2 s1, Segment_2 s2){
    bool intersects = intersection1(s1, s2);
    std::cout << "Do s1 and s2 intersect? Computer says: " << intersects << std::endl;
}

void doIntersectionAndPrint(Segment_2 s1, Segment_2 s2){
    CGAL::Object result = intersection2(s1, s2);

    if (const CGAL::Point_2<Kernel> *ipoint = CGAL::object_cast<CGAL::Point_2<Kernel> >(&result)) {
        // handle the point intersection case with *ipoint.
        std::cout << "Where do s1 and s2 intersect? Computer says: " << *ipoint << std::endl;
    } else {
        if (const CGAL::Segment_2<Kernel> *iseg = CGAL::object_cast<CGAL::Segment_2<Kernel> >(&result)) {
            // handle the segment intersection case with *iseg.
            std::cout << "Where do s1 and s2 intersect? Computer says: " << *iseg << std::endl;
        } else {
            // handle the no intersection case.
            std::cout << "Where do s1 and s2 intersect? Computer says: nowhere" << std::endl;
        }
    }
}

/*
 * main function
 */

int main( int argc, char* argv[]) {

    // create some points in the plane
    Point_2 p1(1,2), p2(4,2);
    Point_2 p3(2,1), p4(4,3);
    Point_2 p5(2,2), p6(6,2);
    Point_2 p7(1,4), p8(2,3);

    // create two segments
    Segment_2 s1(p1, p2);
    Segment_2 s2(p3, p4);
    Segment_2 s3(p5, p6);
    Segment_2 s4(p7, p8);

    echo("Segment 1 and 2, they should intersect at point (3,2)");
    printSegments(s1, s2);
    testForIntersectionAndPrint(s1, s2);
    doIntersectionAndPrint(s1, s2);
    newline();

    echo("Segment 1 and 3, they should intersect at segment (2,2), (4,2)");
    printSegments(s1, s3);
    testForIntersectionAndPrint(s1, s3);
    doIntersectionAndPrint(s1, s3);
    newline();

    echo("Segment 1 and 4, they should not intersect");
    printSegments(s1, s4);
    testForIntersectionAndPrint(s1, s4);
    doIntersectionAndPrint(s1, s4);

    return 0;
}
