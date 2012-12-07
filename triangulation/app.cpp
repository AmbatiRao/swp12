#include <QApplication>
#include "viewer.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Viewer viewer;

    viewer.setWindowTitle("fooo");
    viewer.show();

    return app.exec();
}
