#include "volumedatawidget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

VolumeDataWidget::VolumeDataWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.windowLevel_slider->setValue(ui.histogram_widget->windowLevel() * 100);
	ui.windowWidth_slider->setValue(ui.histogram_widget->windowWidth() * 100);
	ui.windowing_checkBox->setChecked(ui.histogram_widget->showWindowing());

	connect(ui.histogram_widget, &HistogramWidget::windowLevelChanged, this, &VolumeDataWidget::onUpdateWindowLevel);
	connect(ui.histogram_widget, &HistogramWidget::windowWidthChanged, this, &VolumeDataWidget::onUpdateWindowWidth);
}

void VolumeDataWidget::onUpdateWindowLevel(float level)
{
	ui.windowLevel_slider->setValue(level * 100);
}

void VolumeDataWidget::onUpdateWindowWidth(float width)
{
	ui.windowWidth_slider->setValue(width * 100);
}

void VolumeDataWidget::onLoadVolumeData()
{
	// Open the mhd file
	QString mhdFilePath = QFileDialog::getOpenFileName(this, "Open Volume Data", "", "Volume Data Files (*.mhd)");
	if (mhdFilePath.isEmpty())
		return;

	try
	{
		m_volumeDataManager.load(mhdFilePath.toStdString());
	}
	catch (const std::exception& e)
	{
		QMessageBox::warning(this, "Error", QString("Failed to load volume data") + e.what());
		return;
	}

	ui.histogram_widget->load(m_volumeDataManager.histogram());
}
