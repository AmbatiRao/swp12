# This is the CMake script for compiling a CGAL application.

project (convex_hull)

cmake_minimum_required(VERSION 2.6.2)

find_package(CGAL COMPONENTS Core)

if ( CGAL_FOUND )

  include(${CGAL_USE_FILE})

  add_executable  ( convex_hull convex_hull.cpp )

  # Link with CGAL
  target_link_libraries( convex_hull ${CGAL_LIBRARIES} )

else()

  message(STATUS "NOTICE: This demo requires CGAL")

endif()
