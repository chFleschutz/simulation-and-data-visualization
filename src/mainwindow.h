#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow() = default;

public slots:
	void onShowDebugOutput();
	void onMapVisualization();
	void onPandemicSimulation();
	void onVolumeDataVisualization();

private:
	Ui::MainWindowClass ui;
};
