#include "pandemicstatisticswidget.h"

#include <QGridLayout>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QTimer>

PandemicStatisticsWidget::PandemicStatisticsWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setupAreaChart();
	setupPieChart();

	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &PandemicStatisticsWidget::onUpdateStatistics);
	timer->start(100);
}

void PandemicStatisticsWidget::onStartSimulation()
{
	clearStatistics();

	// Set initial values at time 0 so the chart starts correctly
	if (m_crowdSimWidget == nullptr)
		return;

	auto& sim = m_crowdSimWidget->crowdSim();
	int healthy = sim.agentCount(CrowdSim::Agent::State::Healthy);
	int infected = sim.agentCount(CrowdSim::Agent::State::Infected);
	int recovered = sim.agentCount(CrowdSim::Agent::State::Recovered);
	int dead = sim.agentCount(CrowdSim::Agent::State::Dead);
	fillAreaChart(0.0f, healthy, infected, recovered, dead);
}

void PandemicStatisticsWidget::setupPieChart()
{
	m_healthySlice = new QPieSlice("Healthy", 1, this);
	m_healthySlice->setColor(m_healthyColor);

	m_infectedSlice = new QPieSlice("Infected", 1, this);
	m_infectedSlice->setColor(m_infectedColor);

	m_recoveredSlice = new QPieSlice("Recovered", 1, this);
	m_recoveredSlice->setColor(m_recoveredColor);

	m_deadSlice = new QPieSlice("Dead", 1, this);
	m_deadSlice->setColor(m_deadColor);

	m_pieSeries = new QPieSeries(this);
	m_pieSeries->append(m_healthySlice);
	m_pieSeries->append(m_infectedSlice);
	m_pieSeries->append(m_recoveredSlice);
	m_pieSeries->append(m_deadSlice);
	m_pieSeries->setLabelsVisible(true);
	m_pieSeries->setLabelsPosition(QPieSlice::LabelOutside);

	m_pieChart.addSeries(m_pieSeries);
	m_pieChart.legend()->hide();
	m_pieChart.setTitle("Population Distribution");

	ui.pie_chartView->setChart(&m_pieChart);
	ui.pie_chartView->setRenderHint(QPainter::Antialiasing);
}

void PandemicStatisticsWidget::setupAreaChart()
{
	m_healthySeries = new QLineSeries(this);
	m_healthySeries->setName("Healthy");
	m_infectedSeries = new QLineSeries(this);
	m_infectedSeries->setName("Infected");
	m_recoveredSeries = new QLineSeries(this);
	m_recoveredSeries->setName("Recovered");
	m_deadSeries = new QLineSeries(this);
	m_deadSeries->setName("Dead");

	m_deadAreaSeries = new QAreaSeries(m_deadSeries);
	m_deadAreaSeries->setColor(m_deadColor);

	m_infectedAreaSeries = new QAreaSeries(m_infectedSeries, m_deadSeries);
	m_infectedAreaSeries->setColor(m_infectedColor);

	m_recoveredAreaSeries = new QAreaSeries(m_recoveredSeries, m_infectedSeries);
	m_recoveredAreaSeries->setColor(m_recoveredColor);

	m_helthyAreaSeries = new QAreaSeries(m_healthySeries, m_recoveredSeries);
	m_helthyAreaSeries->setColor(m_healthyColor);
	
	m_areaChart.addSeries(m_helthyAreaSeries);
	m_areaChart.addSeries(m_infectedAreaSeries);
	m_areaChart.addSeries(m_recoveredAreaSeries);
	m_areaChart.addSeries(m_deadAreaSeries);

	m_areaChart.createDefaultAxes();
	m_areaChart.axisX()->setTitleText("Time");
	m_areaChart.axisY()->setTitleText("Population");
	m_areaChart.legend()->hide();

	ui.chartView->setChart(&m_areaChart);
	ui.chartView->setRenderHint(QPainter::Antialiasing);
}

void PandemicStatisticsWidget::onUpdateStatistics()
{
	if (m_crowdSimWidget == nullptr)
		return;

	if (!m_crowdSimWidget->isSimulationRunning() || m_crowdSimWidget->isFrozen())
		return;

	auto& sim = m_crowdSimWidget->crowdSim();
	float total = static_cast<float>(sim.agentCount());
	int healthy = sim.agentCount(CrowdSim::Agent::State::Healthy);
	int infected = sim.agentCount(CrowdSim::Agent::State::Infected);
	int recovered = sim.agentCount(CrowdSim::Agent::State::Recovered);
	int dead = sim.agentCount(CrowdSim::Agent::State::Dead);

	m_healthySlice->setValue(healthy);
	m_infectedSlice->setValue(infected);
	m_recoveredSlice->setValue(recovered);
	m_deadSlice->setValue(dead);

	auto time = m_crowdSimWidget->simulationTime();
	fillAreaChart(time, healthy, infected, recovered, dead);

	m_areaChart.axisX()->setRange(0, time);
	m_areaChart.axisY()->setRange(0, total);
}

void PandemicStatisticsWidget::clearStatistics()
{
	m_healthySeries->clear();
	m_infectedSeries->clear();
	m_recoveredSeries->clear();
	m_deadSeries->clear();

	m_healthySlice->setValue(1);
	m_infectedSlice->setValue(1);
	m_recoveredSlice->setValue(1);
	m_deadSlice->setValue(1);
}

void PandemicStatisticsWidget::fillAreaChart(float time, int healthy, int infected, int recovered, int dead)
{
	m_deadSeries->append(time, dead);
	m_infectedSeries->append(time, dead + infected);
	m_recoveredSeries->append(time, dead + infected + recovered);
	m_healthySeries->append(time, dead + infected + recovered + healthy);
}
