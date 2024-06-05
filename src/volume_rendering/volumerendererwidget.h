#pragma once

#include <QWidget>
#include "ui_volumerendererwidget.h"

class VolumeRendererWidget : public QWidget
{
	Q_OBJECT

public:
	VolumeRendererWidget(QWidget *parent = nullptr);
	~VolumeRendererWidget() = default;

private:
	Ui::VolumeRendererWidgetClass ui;
};
