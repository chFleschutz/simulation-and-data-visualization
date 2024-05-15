#include "mainwindow.h"

#include "mapvisualization/mapvisualization.h"
#include "pandemic_sim/pandemicsimwidget.h"
#include "utils/debugoutputdialog.h"
#include "volume_data/volumedatawidget.h"

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

void MainWindow::onVolumeDataVisualization()
{
    setCentralWidget(new VolumeDataWidget(this));
}

void MainWindow::onShowDebugOutput()
{
    DebugOutputDialog* dialog = new DebugOutputDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

