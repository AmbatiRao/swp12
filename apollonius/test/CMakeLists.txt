# This is the CMake script for compiling a CGAL application.

project (test)

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
  add_executable  ( test1 test1.cpp )
  add_executable  ( test2 test2.cpp )
  add_executable  ( test3 test3.cpp )
  add_executable  ( test4 test4.cpp )
  add_executable  ( test5 test5.cpp )

  target_link_libraries( test1 ${CGAL_LIBRARIES}  ${CGAL_3RD_PARTY_LIBRARIES})

  target_link_libraries( test2 ${CGAL_LIBRARIES}  ${CGAL_3RD_PARTY_LIBRARIES})

  target_link_libraries( test3 ${CGAL_LIBRARIES}  ${CGAL_3RD_PARTY_LIBRARIES})

  target_link_libraries( test4 ${CGAL_LIBRARIES}  ${CGAL_3RD_PARTY_LIBRARIES})
  target_link_libraries( test4 ${QT_LIBRARIES} )

  target_link_libraries( test5 ${CGAL_LIBRARIES}  ${CGAL_3RD_PARTY_LIBRARIES})
  target_link_libraries( test5 ${QT_LIBRARIES} )

else()

  message(STATUS "NOTICE: This demo requires CGAL and Qt4, and will not be compiled.")

endif()
