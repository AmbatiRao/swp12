#include <QGLViewer/qglviewer.h>

#include "typedefs.h"

class Viewer : public QGLViewer {
    private:
        Triangulation triangulation;
    protected:
        virtual void draw();
        virtual void init();
        virtual QString helpString() const;
        void drawVertex(const Point&, const QColor& clr, float r);
        void drawEdge(const Point& from, const Point& to, const QColor& clr, float r);
};
