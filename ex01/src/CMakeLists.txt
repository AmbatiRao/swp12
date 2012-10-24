# This is the CMake script for compiling a CGAL application.

project (exercise1)

cmake_minimum_required(VERSION 2.6.2)

find_package(CGAL COMPONENTS Core)

if ( CGAL_FOUND )

  include(${CGAL_USE_FILE})

  add_executable  ( exercise1 exercise1.cpp )

  # Link with CGAL
  target_link_libraries( exercise1 ${CGAL_LIBRARIES} )

else()

  message(STATUS "NOTICE: This demo requires CGAL")

endif()
