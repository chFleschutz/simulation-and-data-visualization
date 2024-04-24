#include "crowdsimwidget.h"

#include <QPainter>
#include <QTimer>

CrowdSimWidget::CrowdSimWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setMinimumSize(200, 200);

	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &CrowdSimWidget::updateSimulation);
	timer->start(1000 / 60); // 60 FPS

	m_timer.start();
}

void CrowdSimWidget::startSimulation()
{
	m_crowdSim.spawnAgents(100);
}

void CrowdSimWidget::updateSimulation()
{
	auto time = m_timer.restart() / 1000.0f;
	emit FPSChanged(1.0f / time);

	m_crowdSim.update(time);
	update();
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
			color = Qt::green;
			break;
		case CrowdSim::Agent::State::Infected:
			color = Qt::red;
			break;
		case CrowdSim::Agent::State::Recovered:
			color = Qt::blue;
			break;
		case CrowdSim::Agent::State::Dead:
			color = Qt::black;
			break;
		}

		painter.setBrush(color);
		painter.drawEllipse(agent.position.x(), agent.position.y(), m_agentSize, m_agentSize);
	}
}
