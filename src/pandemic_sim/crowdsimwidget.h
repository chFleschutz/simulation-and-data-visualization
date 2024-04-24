#pragma once

#include "crowdSim.h"

#include "ui_crowdsimwidget.h"

#include <QWidget>
#include <QElapsedTimer>

class CrowdSimWidget : public QWidget
{
	Q_OBJECT

public:
	CrowdSimWidget(QWidget* parent = nullptr);
	~CrowdSimWidget() = default;

signals:
	void FPSChanged(float fps);

public slots:
	void onStartSimulation();
	void onStopSimulation();
	void onUpdateSimulation();
	void onAgentCountChanged(int count);
	void onInfectionDistanceChanged(int distance);
	void onAgentSpeedChanged(int speed);
	void onFreezeChanged(bool freeze);

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	void drawAgents(QPainter& painter);

	Ui::CrowdSimWidgetClass ui;

	CrowdSim m_crowdSim;
	QElapsedTimer m_timer;

	float m_agentSize = 10.0f;
	int m_agentCount = 100;
	bool m_freeze = false;
};
