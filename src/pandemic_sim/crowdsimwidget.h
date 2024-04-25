#pragma once

#include "crowdSim.h"
#include "utils/stopwatch.h"

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

	// Returns true if the simulation is currently frozen
	bool isFrozen() const { return m_freeze; }

	// Returns the current simulation time in seconds
	float simulationTime() const;

signals:
	void FPSChanged(float fps);
	void simulationStarted();
	void simulationStopped();

public slots:
	void onStartSimulation();
	void onStopSimulation();
	void onFreezeChanged(bool freeze);
	void onUpdateSimulation();
	void onAgentCountChanged(int count);
	void onInfectionDistanceChanged(int distance);
	void onAgentSpeedChanged(int speed);
	void onInitalInfectedChanged(int infected);
	void onRecoveryTimeChanged(int time);
	void onDeathRateChanged(int rate);

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	void drawAgents(QPainter& painter);

	QElapsedTimer m_FPSTimer;

	CrowdSim m_crowdSim;
	StopWatch m_simTimer;
	float m_simTimeOffset = 0.0f; // Correction for time when simulation is paused

	float m_agentSize = 10.0f;
	bool m_freeze = false;
	bool m_simulationRunning = false;
};
