#include "volumedatawidget.h"

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

VolumeDataWidget::VolumeDataWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.windowing_checkBox->setChecked(ui.histogram_widget->showWindowing());

	onSliceLevelChanged(0);
}

void VolumeDataWidget::onUpdateWindowLevel(float level)
{
	ui.sliceViewer->setWindowLevel(level);
	ui.windowLevel_label->setText(QString::number(level, 'f', 2));
}

void VolumeDataWidget::onUpdateWindowWidth(float width)
{
	ui.sliceViewer->setWindowWidth(width);
	ui.windowWidth_label->setText(QString::number(width, 'f', 2));
}

void VolumeDataWidget::onSliceLevelChanged(float level)
{
	ui.sliceLevel_label->setText(QString::number(level, 'f', 1));
}

void VolumeDataWidget::onUpdateWindowing(bool enable)
{
	ui.sliceViewer->setWindowing(enable);
}

void VolumeDataWidget::onLoadVolumeData()
{
	QString mhdFilePath = QFileDialog::getOpenFileName(this, "Open Volume Data", "", "Volume Data Files (*.mhd)");
	if (mhdFilePath.isEmpty())
		return;

	loadFile(mhdFilePath.toStdString());

	ui.file_comboBox->setCurrentIndex(-1);
}

void VolumeDataWidget::onLoadDefaultFile(int index)
{
	switch (index)
	{
	case 0:
		loadFile(QFileInfo(":/assets/volume/smallHeart.mhd").absoluteFilePath().toStdString());
		break;
	default:
		break;
	}
}

void VolumeDataWidget::loadFile(const std::filesystem::path& mhdFile)
{
	try
	{
		m_volumeDataManager.load(mhdFile);
	}
	catch (const std::exception& e)
	{
		QMessageBox::warning(this, "Error", QString("Failed to load volume data\n") + e.what());
		return;
	}

	ui.histogram_widget->load(m_volumeDataManager.histogram());
	ui.sliceViewer->load(m_volumeDataManager.data());
}
