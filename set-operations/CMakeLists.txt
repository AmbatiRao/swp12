# This is the CMake script for compiling a CGAL application.

project (intersect)

cmake_minimum_required(VERSION 2.6.2)

find_package(CGAL COMPONENTS Core)

if ( CGAL_FOUND )

  include(${CGAL_USE_FILE})

  add_executable  ( intersect main.cpp Token.cpp TokenStream.cpp Parser.cpp)

  # Link with CGAL
  target_link_libraries( intersect ${CGAL_LIBRARIES} )

else()

  message(STATUS "NOTICE: This demo requires CGAL")

endif()
