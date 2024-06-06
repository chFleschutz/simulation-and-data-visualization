#pragma once

#include <QWidget>
#include "ui_volumerendererwidget.h"

class VolumeRendererWidget : public QWidget
{
	Q_OBJECT

public:
	VolumeRendererWidget(QWidget *parent = nullptr);
	~VolumeRendererWidget() = default;

public slots:
	void onRenderModeEntryPoints();
	void onRenderModeExitPoints();
	void onRenderModeTexture();
	void onRenderModeVolumeMIP();
	void onRenderModeVolumeDRR();

private:
	Ui::VolumeRendererWidgetClass ui;
};
