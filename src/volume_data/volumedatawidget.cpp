#include "volumedatawidget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

VolumeDataWidget::VolumeDataWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

void VolumeDataWidget::createHistogram()
{
	const auto& histogram = m_volumeDataManager.histogram();
	auto& imageRenderer = ui.histogram->renderer();

	auto width = 1024;
	auto height = 512;
	imageRenderer.create(QSize(width, height));

	float maxValue = m_volumeDataManager.maxValue();
	float maxCount = m_volumeDataManager.maxCount();
	auto& image = imageRenderer.originalImage();
	for (auto [key, value] : histogram)
	{
		auto x = static_cast<float>(key) / maxValue * width;
		auto y = static_cast<float>(value) / maxCount * height;

		for (auto i = 0; i < y; ++i)
		{
			image.setPixel(x, height - i, qRgb(255, 0, 0));
		}
	}

	imageRenderer.update();
	update();
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

	createHistogram();
}
