#pragma once

#include "ui_volumedatawidget.h"
#include "slice_viewer/volumedatamanager.h"

#include <QWidget>

class VolumeDataWidget : public QWidget
{
	Q_OBJECT

public:
	VolumeDataWidget(QWidget *parent = nullptr);
	~VolumeDataWidget() = default;

public slots:
	void onLoadVolumeData();
	void onUpdateWindowLevel(float level);
	void onUpdateWindowWidth(float width);

private:
	Ui::VolumeDataWidgetClass ui;

	VolumeDataManager m_volumeDataManager;
};
