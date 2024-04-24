#include "crowdsimwidget.h"

#include <QPainter>
#include <QTimer>

CrowdSimWidget::CrowdSimWidget(QWidget* parent)
	: QWidget(parent)
{
	setMinimumSize(200, 200);

	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &CrowdSimWidget::onUpdateSimulation);
	timer->start(1000.0f / 60.0f); // 60 FPS

	m_FPSTimer.start();
}

float CrowdSimWidget::simulationTime() const
{
	return m_simTimer.elapsedTime();
}

void CrowdSimWidget::onStartSimulation()
{
	m_crowdSim.spawnAgents(m_agentCount);
	m_simulationRunning = true;

	m_simTimer.start();

	emit simulationStarted();
	update();
}

void CrowdSimWidget::onStopSimulation()
{
	m_crowdSim.clearAgents();
	m_simulationRunning = false;

	m_simTimer.reset();

	emit simulationStopped();
	update();
}

void CrowdSimWidget::onFreezeChanged(bool freeze)
{
	m_freeze = freeze;

	if (m_freeze)
		m_simTimer.pause();
	else
		m_simTimer.start();
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

void CrowdSimWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(Qt::white);
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
	painter.setPen(QColor(50, 50, 50, 255));

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
