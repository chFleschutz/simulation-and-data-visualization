#include "simulationanddatavisualization.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    simulationanddatavisualization w;
    w.show();
    return a.exec();
}
