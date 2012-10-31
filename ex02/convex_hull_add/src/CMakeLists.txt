# This is the CMake script for compiling a CGAL application.

project (convex_hull)

cmake_minimum_required(VERSION 2.6.2)

find_package(CGAL COMPONENTS Qt4)

include(${CGAL_USE_FILE})


set( QT_USE_QTXML    TRUE )
set( QT_USE_QTMAIN   TRUE )
set( QT_USE_QTSCRIPT  TRUE )
set( QT_USE_QTOPENGL  TRUE )

find_package(Qt4)

if ( CGAL_FOUND AND CGAL_Qt4_FOUND AND QT4_FOUND )

  include(${QT_USE_FILE})

  # Make sure to correct this line and duplicate it according to the project.
  add_executable  ( main main.cpp )

  # Link with Qt libraries
  target_link_libraries( main ${QT_LIBRARIES} )
  # Link with CGAL
  target_link_libraries( main ${CGAL_LIBRARIES}  ${CGAL_3RD_PARTY_LIBRARIES})

else()

  message(STATUS "NOTICE: This demo requires CGAL and Qt4, and will not be compiled.")

endif()
