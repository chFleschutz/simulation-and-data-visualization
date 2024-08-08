#include "volumerendererwidget.h"

#include <QFileInfo>

VolumeRendererWidget::VolumeRendererWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.volume_renderer, &VolumeRenderer::rendererReady, this, &VolumeRendererWidget::onRendererReady);
	connect(ui.transferFunctionWidget, &TransferFunctionWidget::transferFunctionChanged, 
		ui.volume_renderer, &VolumeRenderer::setTransferFunction, Qt::DirectConnection);
}

void VolumeRendererWidget::onRendererReady()
{
	auto path = QFileInfo(":/assets/volume/smallHeart.mhd").absoluteFilePath().toStdString();
	VolumeFileLoader::load(path, m_volumeData);
	ui.volume_renderer->setVolumeData(m_volumeData);

	m_histogram.update(m_volumeData.data, 4096);
	ui.transferFunctionWidget->initialize();
	ui.transferFunctionWidget->setHistogram(m_histogram);
}

void VolumeRendererWidget::onRenderModeEntryPoints()
{
	ui.volume_renderer->setRenderMode(VolumeRenderer::RenderMode::EntryPoints);
}

void VolumeRendererWidget::onRenderModeTexture()
{
	ui.volume_renderer->setRenderMode(VolumeRenderer::RenderMode::Texture);
}

void VolumeRendererWidget::onRenderModeVolumeMIP()
{
	ui.volume_renderer->setRenderMode(VolumeRenderer::RenderMode::VolumeMIP);
}

void VolumeRendererWidget::onRenderModeVolumeDRR()
{
	ui.volume_renderer->setRenderMode(VolumeRenderer::RenderMode::VolumeDRR);
}

void VolumeRendererWidget::onRenderModeVolumeTFN()
{
	ui.volume_renderer->setRenderMode(VolumeRenderer::RenderMode::VolumeTFN);
}

void VolumeRendererWidget::onRenderModeVolumePhong()
{
	ui.volume_renderer->setRenderMode(VolumeRenderer::RenderMode::VolumePhong);
}
