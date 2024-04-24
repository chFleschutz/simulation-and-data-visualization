#pragma once

#include <QWidget>
#include "ui_pandemicstatisticswidget.h"
#include <QtCharts/QPieSlice>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QAreaSeries>

#include "pandemic_sim/crowdsimwidget.h"

class PandemicStatisticsWidget : public QWidget
{
	Q_OBJECT

public:
	PandemicStatisticsWidget(QWidget* parent = nullptr);
	~PandemicStatisticsWidget();

	void setCrowdSimWidget(CrowdSimWidget* crowdSimWidget) { m_crowdSimWidget = crowdSimWidget; }

public slots:
	void onUpdateStatistics();
	void onStartSimulation();

private:
	void setupPieChart();
	void setupAreaChart();
	void clearStatistics();
	void fillAreaChart(float time, int healthy, int infected, int recovered, int dead);

	Ui::PandemicStatisticsWidgetClass ui;

	CrowdSimWidget* m_crowdSimWidget = nullptr;

	QChart m_pieChart;
	QPieSeries* m_pieSeries;
	QPieSlice* m_healthySlice;
	QPieSlice* m_infectedSlice;
	QPieSlice* m_recoveredSlice;
	QPieSlice* m_deadSlice;

	QChart m_areaChart;
	QAreaSeries* m_helthyAreaSeries;
	QAreaSeries* m_infectedAreaSeries;
	QAreaSeries* m_recoveredAreaSeries;
	QAreaSeries* m_deadAreaSeries;
	QLineSeries* m_healthySeries;
	QLineSeries* m_infectedSeries;
	QLineSeries* m_recoveredSeries;
	QLineSeries* m_deadSeries;

	QColor m_healthyColor = QColor(0, 255, 0, 100);
	QColor m_infectedColor = QColor(255, 0, 0, 100);
	QColor m_recoveredColor = QColor(0, 0, 255, 100);
	QColor m_deadColor = QColor(50, 50, 50, 100);
};
