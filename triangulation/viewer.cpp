#include "viewer.h"
#include <CGAL/glu.h>
#include <CGAL/point_generators_3.h>

void Viewer::draw() {


    Triangulation::Finite_vertices_iterator iter =
        triangulation.finite_vertices_begin();
    for(iter; iter != triangulation.finite_vertices_end(); iter++) {
        drawVertex(iter->point(), QColor(255, 150, 0), 1);
    }

    Triangulation::Finite_edges_iterator iter2 =
        triangulation.finite_edges_begin();
    for(iter2; iter2 != triangulation.finite_edges_end(); iter2++) {
        Segment_3 segment = triangulation.segment(*iter2);
        drawEdge(
                segment.vertex(0),
                segment.vertex(1),
                QColor(255, 150, 0),
                0.5
                );
    }

}

void Viewer::init() {

    ::glShadeModel(GL_SMOOTH);

    ::glClearDepth(1.0f);
    ::glEnable(GL_DEPTH_TEST);
    ::glDepthFunc(GL_LEQUAL);
    ::glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    ::glEnable(GL_BLEND);
    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ::glEnable(GL_POINT_SMOOTH);
    ::glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    ::glEnable(GL_LINE_SMOOTH);
    ::glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    //CGAL specific init
    std::list<Point> points;

    triangulation = Triangulation(points.begin(), points.end());

    //for(int x=0; x < 10; x++) {
    //    for(int y=0; y < 10; y++) {
    //        for(int z=0; z < 10; z++) {
    //            Point p = Point(x*10, y*10, z*10);
    //            triangulation.insert(p);
    //            //drawVertex(p, QColor(255, 150, 0), 1);
    //        }
    //    }
    //}

    //triangulation.insert(Point(0, 0, 0));
    //triangulation.insert(Point(0, 10, 0));
    //triangulation.insert(Point(10, 10, 0));

    CGAL::Random_points_in_sphere_3<Point> rnd(20);

    for (int i = 0; i < 10; i++) {
        triangulation.insert(*rnd++);
    }

}


void Viewer::drawVertex(const Point& p, const QColor& clr, float r) {


    /* Draw vertices as 3D balls */
    GLboolean lighting, colorMaterial;
    ::glGetBooleanv( GL_LIGHTING, &lighting );
    ::glGetBooleanv( GL_COLOR_MATERIAL, &colorMaterial );
    ::glEnable( GL_LIGHTING );
    ::glDisable(GL_COLOR_MATERIAL);

    float color[4];
    color[0] = clr.redF();
    color[1] = clr.greenF();
    color[2] = clr.blueF();
    color[3] = clr.alphaF();

    // move to the point
    ::glPushMatrix();
    ::glTranslatef( p.x(), p.y(), p.z() );

    // draw
    GLUquadricObj* quadratic = ::gluNewQuadric();	// Create A Pointer To The Quadric Object
    ::gluQuadricNormals( quadratic, GLU_SMOOTH );	// Create Smooth Normals
    ::glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color );
    ::gluSphere( quadratic, r, 16, 16 );

    // move back to origin
    ::glPopMatrix();

    if ( colorMaterial )
      ::glEnable( GL_COLOR_MATERIAL );
    if ( !lighting )
      ::glDisable( GL_LIGHTING );

}

void Viewer::drawEdge(const Point& from, const Point& to, const QColor& clr, float r)
{

  /* Draw edges as 3D cylinders */
  GLboolean lighting, colorMaterial;
  ::glGetBooleanv( GL_LIGHTING, &lighting );
  ::glGetBooleanv( GL_COLOR_MATERIAL, &colorMaterial );
  ::glEnable( GL_LIGHTING );
  ::glDisable(GL_COLOR_MATERIAL);

  float color[4];
  color[0] = clr.redF();
  color[1] = clr.greenF();
  color[2] = clr.blueF();
  color[3] = clr.alphaF();

  Vector_3 v = to - from;

  // compute the length of the edge
  // method 1:
   //  float length = sqrt( CGAL::squared_distance( from, to ) );
  // method 2:
  float length = sqrt( v.squared_length() );

  // normalize
  v = v / length;
  // compute the angle: cos theta = v.z/1.0
  GLfloat angle = acos( v.z() ) / 3.1415927 * 180;

  ::glPushMatrix();

  // move to "from" point
  ::glTranslatef( from.x(), from.y(), from.z() );
  // rotate from z-axis to from-->to
  //  axis: cross product of z-axis and from-->to
  ::glRotatef( angle, -v.y(), v.x(), 0.0f );
  // draw
  GLUquadricObj* quadratic = ::gluNewQuadric();	// Create A Pointer To The Quadric Object
  ::gluQuadricNormals( quadratic, GLU_SMOOTH );	// Create Smooth Normals
  ::glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color );
  // gluCylinder draws a cylinder oriented along the z-axis
  ::gluCylinder( quadratic, r, r, length, 16, 4 );

  // move back to origin
  ::glPopMatrix();

  if ( colorMaterial )
    ::glEnable( GL_COLOR_MATERIAL );
  if ( !lighting )
    ::glDisable( GL_LIGHTING );

}


QString Viewer::helpString() const {
    QString text("foo");
    return text;
}
