#include "crowdsimwidget.h"

#include <QPainter>
#include <QTimer>

CrowdSimWidget::CrowdSimWidget(QWidget* parent)
	: QWidget(parent)
{
	setMinimumSize(200, 200);

	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &CrowdSimWidget::onUpdateSimulation);
	timer->start(1000 / 60); // 60 FPS

	m_FPSTimer.start();
}

void CrowdSimWidget::onStartSimulation()
{
	m_crowdSim.spawnAgents(m_agentCount);
	m_simulationTimer.start();
	m_simulationRunning = true;

	emit simulationStarted();
	update();
}

void CrowdSimWidget::onStopSimulation()
{
	m_crowdSim.clearAgents();
	m_simulationTimer.invalidate();
	m_simulationRunning = false;
	
	emit simulationStopped();
	update();
}

void CrowdSimWidget::onUpdateSimulation()
{
	auto time = m_FPSTimer.restart() / 1000.0f;
	emit FPSChanged(1.0f / time);

	if (!m_freeze)
	{
		m_crowdSim.update(time);
		update();
	}
}

void CrowdSimWidget::onAgentCountChanged(int count)
{
	m_agentCount = count;
}

void CrowdSimWidget::onInfectionDistanceChanged(int distance)
{
	m_crowdSim.setCollisionDistance(distance);
}

void CrowdSimWidget::onAgentSpeedChanged(int speed)
{
	m_crowdSim.setAgentSpeed(speed);
}

void CrowdSimWidget::onFreezeChanged(bool freeze)
{
	m_freeze = freeze;
}

void CrowdSimWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(Qt::black);
	painter.setBrush(Qt::white);
	painter.drawRect(0, 0, width(), height());

	drawAgents(painter);
}

void CrowdSimWidget::resizeEvent(QResizeEvent* event)
{
	m_crowdSim.setBounds(width() - m_agentSize, height() - m_agentSize);
}

void CrowdSimWidget::drawAgents(QPainter& painter)
{
	for (const auto& agent : m_crowdSim.agents())
	{
		QColor color;
		switch (agent.state)
		{
		case CrowdSim::Agent::State::Healthy:
			color = QColor(0, 255, 0, 140);
			break;
		case CrowdSim::Agent::State::Infected:
			color = QColor(255, 0, 0, 140);
			break;
		case CrowdSim::Agent::State::Recovered:
			color = QColor(0, 0, 255, 140);
			break;
		case CrowdSim::Agent::State::Dead:
			color = QColor(50, 50, 50, 140);
			break;
		}

		painter.setBrush(color);
		painter.drawEllipse(agent.position.x(), agent.position.y(), m_agentSize, m_agentSize);
	}
}
