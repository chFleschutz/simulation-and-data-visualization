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
	void createHistogram();
	void drawHistogram(const Histogram<uint16_t>& histogram, QImage& image);

	Ui::VolumeDataWidgetClass ui;

	VolumeDataManager m_volumeDataManager;

	QColor m_linearColor = QColor(150, 150, 150);
	QColor m_logarithmicColor = QColor(200, 200, 200);
};
