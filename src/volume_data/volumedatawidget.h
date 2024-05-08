#pragma once

#include "ui_volumedatawidget.h"
#include "volume_data/volumedatamanager.h"

#include <QWidget>

class VolumeDataWidget : public QWidget
{
	Q_OBJECT

public:
	VolumeDataWidget(QWidget *parent = nullptr);
	~VolumeDataWidget() = default;

public slots:
	void onLoadVolumeData();

private:
	Ui::VolumeDataWidgetClass ui;

	VolumeDataManager m_volumeDataManager;
};
