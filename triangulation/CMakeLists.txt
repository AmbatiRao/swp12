# This is the CMake script for compiling a CGAL application.

project (triangulaion)

cmake_minimum_required(VERSION 2.6.2)

find_package(CGAL COMPONENTS Qt4)

include(${CGAL_USE_FILE})

set( QT_USE_QTXML    TRUE )
set( QT_USE_QTMAIN   TRUE )
set( QT_USE_QTSCRIPT  TRUE )
set( QT_USE_QTOPENGL  TRUE )

find_package(Qt4)
find_package(OpenGL)

if(QT4_FOUND)
  include(${QT_USE_FILE})
  find_package(QGLViewer)
endif(QT4_FOUND)

if ( CGAL_FOUND AND QT4_FOUND AND OPENGL_FOUND AND QGLVIEWER_FOUND )

  add_executable ( triangulation triangulation.cpp )
  add_executable ( app app.cpp viewer.cpp)

  # Link with CGAL
  target_link_libraries( triangulation ${CGAL_LIBRARIES} )
  
  target_link_libraries( app ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})
  target_link_libraries( app ${QT_LIBRARIES} ${QGLVIEWER_LIBRARIES} )
  target_link_libraries( app ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} )


else()
  message(STATUS "NOTICE: This demo requires CGAL and Qt4, and will not be compiled.")

endif()
