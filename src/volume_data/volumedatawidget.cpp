#include "volumedatawidget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

VolumeDataWidget::VolumeDataWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
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
		auto errorMsg = QString("Failed to load volume data") + e.what();
		QMessageBox::warning(this, "Error", errorMsg);
	}

}
