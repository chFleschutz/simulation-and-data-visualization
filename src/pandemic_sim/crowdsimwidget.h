#pragma once

#include "crowdSim.h"

#include <QWidget>
#include <QElapsedTimer>

class CrowdSimWidget : public QWidget
{
	Q_OBJECT

public:
	CrowdSimWidget(QWidget* parent = nullptr);
	~CrowdSimWidget() = default;

	CrowdSim& crowdSim() { return m_crowdSim; }

	// Returns true if the simulation is currently running
	bool isSimulationRunning() const { return m_simulationRunning; }

	// Returns the current simulation time in seconds
	float simulationTime() const { return m_simulationTimer.elapsed() / 1000.0f; }

signals:
	void FPSChanged(float fps);
	void simulationStarted();
	void simulationStopped();

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

	CrowdSim m_crowdSim;
	QElapsedTimer m_FPSTimer;
	QElapsedTimer m_simulationTimer;

	float m_agentSize = 10.0f;
	int m_agentCount = 100;
	bool m_freeze = false;
	bool m_simulationRunning = false;
};
