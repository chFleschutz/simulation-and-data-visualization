#include "mainwindow.h"

#include "mapvisualization/mapvisualization.h"
#include "pandemic_sim/pandemicsimwidget.h"
#include "slice_viewer/volumedatawidget.h"
#include "utils/debugoutputdialog.h"
#include "volume_rendering/volumerendererwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    qInstallMessageHandler(&MainWindow::messageHandler);
}

void MainWindow::messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    if (type == QtWarningMsg || type == QtCriticalMsg || type == QtFatalMsg)
	{
        auto mainwindow = qobject_cast<MainWindow*>(QApplication::activeWindow());
        mainwindow->onShowDebugOutput();

        // Forward the message to the DebugOutputDialog
        DebugOutputDialog::messageHandler(type, context, msg);
	}
}

void MainWindow::onMapVisualization()
{
    setCentralWidget(new MapVisualization(this));
}

void MainWindow::onPandemicSimulation()
{
    setCentralWidget(new PandemicSimWidget(this));
}

void MainWindow::onSliceViewer()
{
    setCentralWidget(new VolumeDataWidget(this));
}

void MainWindow::onVolumeRendering()
{
    setCentralWidget(new VolumeRendererWidget(this));
}

void MainWindow::onShowDebugOutput()
{
    DebugOutputDialog* dialog = new DebugOutputDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

