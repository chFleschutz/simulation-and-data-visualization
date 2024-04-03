#pragma once

#include "ui_mapvisualization.h"

#include "visualizer/shepard.h"
#include "visualizer/voronoi.h"
#include "visualizer/shepardfrankielittle.h"

#include <QWidget>

class MapVisualization : public QWidget
{
	Q_OBJECT

public:
	MapVisualization(QWidget* parent = nullptr) : QWidget(parent)
	{
		ui.setupUi(this);

		connect(ui.voronoiButton, &QPushButton::clicked, this, &MapVisualization::setVoronoi);
		connect(ui.shepardButton, &QPushButton::clicked, this, &MapVisualization::setShepard);
		connect(ui.shepardFrankieLittleButton, &QPushButton::clicked, this, &MapVisualization::setShepardFrankieLittle);

		ui.mapRenderer->setVisualizer<Voronoi>(); // Start with Voronoi
	}

	void setVoronoi()
	{
		ui.mapRenderer->setVisualizer<Voronoi>();
	}

	void setShepard()
	{
		ui.mapRenderer->setVisualizer<Shepard>();
	}

	void setShepardFrankieLittle()
	{
		ui.mapRenderer->setVisualizer<ShepardFrankieLittle>();
	}

private:
	Ui::MapVisualizationClass ui;
};
