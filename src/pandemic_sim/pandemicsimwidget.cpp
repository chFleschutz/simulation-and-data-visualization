#include "pandemicsimwidget.h"

PandemicSimWidget::PandemicSimWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.statistics_widget->setCrowdSimWidget(ui.crowdSimWidget);

	connect(ui.crowdSimWidget, &CrowdSimWidget::FPSChanged, this, &PandemicSimWidget::onUpdateFPS);
}

void PandemicSimWidget::onUpdateFPS(float fps)
{
	ui.fps_counter_label->setText(QString::number(fps, 'f', 1));
}

