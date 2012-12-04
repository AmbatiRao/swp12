#include "viewer.h"
#include <CGAL/glu.h>

void Viewer::draw() {

    std::list<Point> points;

    Triangulation triangulation(points.begin(), points.end());

    for(int x=0; x < 10; x++) {
        for(int y=0; y < 10; y++) {
            for(int z=0; z < 10; z++) {
                Point p = Point(x*10, y*10, z*10);
                triangulation.insert(p);
                drawVertex(p, QColor(255, 150, 0), 1);
            }
        }
    }
}

void Viewer::init() {
    ::glShadeModel(GL_SMOOTH);

    ::glEnable(GL_BLEND);
    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ::glEnable(GL_POINT_SMOOTH);
    ::glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    ::glClearDepth(1.0f);
    ::glEnable(GL_DEPTH_TEST);
    ::glDepthFunc(GL_LEQUAL);
    ::glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

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

QString Viewer::helpString() const {
    QString text("foo");
    return text;
}
