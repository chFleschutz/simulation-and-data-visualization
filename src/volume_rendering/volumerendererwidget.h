#pragma once

#include "slice_viewer/volumedata.h"
#include "ui_volumerendererwidget.h"
#include "volume_rendering/histogram.h"

#include <QWidget>

class VolumeRendererWidget : public QWidget
{
	Q_OBJECT

public:
	VolumeRendererWidget(QWidget *parent = nullptr);
	~VolumeRendererWidget() = default;

public slots:
	void onRendererReady();

	void onRenderModeEntryPoints();
	void onRenderModeTexture();
	void onRenderModeVolumeMIP();
	void onRenderModeVolumeDRR();
	void onRenderModeVolumeTFN();

private:
	Ui::VolumeRendererWidgetClass ui;

	VolumeData m_volumeData;
	Histogram m_histogram;
};
