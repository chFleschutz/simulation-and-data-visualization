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

void VolumeDataWidget::createHistogram()
{
	auto& imageRenderer = ui.histogram->renderer();
	imageRenderer.create(QSize(1024, 512));
	imageRenderer.originalImage().fill(Qt::white);

	drawHistogram(m_volumeDataManager.histogram(), imageRenderer.originalImage());

	ui.histogram->updateSize();
	update();
}

void VolumeDataWidget::drawHistogram(const Histogram<uint16_t>& histogram, QImage& image)
{
	QPainter painter(&image);

	auto maxValue = histogram.maxValue;
	auto maxCount = histogram.maxCount;

	for (auto [key, value] : histogram.data)
	{
		auto percentX = static_cast<float>(key) / static_cast<float>(maxValue);
		auto percentY = static_cast<float>(value) / static_cast<float>(maxCount);
		auto logPercentY = std::log(value) / std::log(maxCount);

		auto x = percentX * image.width();
		auto linearY = percentY * image.height();
		auto logY = logPercentY * image.height();

		painter.setPen(QPen(m_linearColor, 1));
		painter.drawLine(x, image.height(), x, image.height() - linearY);

		painter.setPen(QPen(m_logarithmicColor, 1));
		painter.drawLine(x, image.height() - linearY, x, image.height() - logY);
	}
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

	ui.histogram_widget->load(m_volumeDataManager.histogram());
	//createHistogram();
}
