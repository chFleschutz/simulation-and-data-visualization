#include "mainwindow.h"

#include "utils/debugoutputdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void MainWindow::onShowDebugOutput()
{
    DebugOutputDialog* dialog = new DebugOutputDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

