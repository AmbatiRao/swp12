#include <QGLViewer/qglviewer.h>

#include "typedefs.h"

class Viewer : public QGLViewer {
    protected:
        virtual void draw();
        virtual void init();
        virtual QString helpString() const;
        void drawVertex(const Point&, const QColor& clr, float r);
};
