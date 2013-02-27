#ifndef _CGAL_H
#define _CGAL_H

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/enum.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2                                   Point;
typedef CGAL::Polygon_2<Kernel>                           Polygon;
typedef CGAL::Polygon_with_holes_2<Kernel>                Polygon_with_holes;
typedef CGAL::Polygon_set_2<Kernel>                       Polygon_set;

/* Copied from examples: */
#include <iostream>

//-----------------------------------------------------------------------------
// Pretty-print a CGAL polygon.
//
template<class Kernel, class Container>
void print_polygon (const CGAL::Polygon_2<Kernel, Container>& P)
{
    typename CGAL::Polygon_2<Kernel, Container>::Vertex_const_iterator vit;
    
    std::cerr << "[ " << P.size() << " vertices:";
    for (vit = P.vertices_begin(); vit != P.vertices_end(); ++vit)
        std::cerr << " (" << *vit << ')';
    std::cerr << " ]" << std::endl;
    
    return;
}

//-----------------------------------------------------------------------------
// Pretty-print a polygon with holes.
//
template<class Kernel, class Container>
void print_polygon_with_holes
(const CGAL::Polygon_with_holes_2<Kernel, Container>& pwh)
{
    if (! pwh.is_unbounded())
    {
        std::cerr << "{ Outer boundary = ";
        print_polygon (pwh.outer_boundary());
    }
    else
        std::cerr << "{ Unbounded polygon." << std::endl;
    
    typename CGAL::Polygon_with_holes_2<Kernel,Container>::
    Hole_const_iterator hit;
    unsigned int k = 1;
    
    std::cerr << " " << pwh.number_of_holes() << " holes:" << std::endl;
    for (hit = pwh.holes_begin(); hit != pwh.holes_end(); ++hit, ++k)
    {
        std::cerr << " Hole #" << k << " = ";
        print_polygon (*hit);
    }
    std::cerr << " }" << std::endl;
    
    return;
}

#endif // _CGAL_H
