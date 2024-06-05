#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow() = default;

	static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

public slots:
	void onShowDebugOutput();
	void onMapVisualization();
	void onPandemicSimulation();
	void onSliceViewer();
	void onVolumeRendering();

private:
	Ui::MainWindowClass ui;
};
