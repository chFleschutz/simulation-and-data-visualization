#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_simulationanddatavisualization.h"

class simulationanddatavisualization : public QMainWindow
{
    Q_OBJECT

public:
    simulationanddatavisualization(QWidget *parent = nullptr);
    ~simulationanddatavisualization();

private:
    Ui::simulationanddatavisualizationClass ui;
};
