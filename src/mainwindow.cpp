#include "mainwindow.h"

#include "utils/debugoutputdialog.h"
#include "mapvisualization/mapvisualization.h"
#include "pandemic_sim/pandemicsimwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void MainWindow::onMapVisualization()
{
    setCentralWidget(new MapVisualization(this));
}

void MainWindow::onPandemicSimulation()
{
    setCentralWidget(new PandemicSimWidget(this));
}

void MainWindow::onShowDebugOutput()
{
    DebugOutputDialog* dialog = new DebugOutputDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

