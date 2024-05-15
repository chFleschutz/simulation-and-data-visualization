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
	void onUpdateWindowLevel(float level);
	void onUpdateWindowWidth(float width);
	void onSliceLevelChanged(float level);
	void onUpdateWindowing(bool enable);
	
	void onLoadVolumeData();
	void onLoadDefaultFile(int index);

private:
	void loadFile(const std::filesystem::path& mhdFile);

	Ui::VolumeDataWidgetClass ui;

	VolumeDataManager m_volumeDataManager;
};
