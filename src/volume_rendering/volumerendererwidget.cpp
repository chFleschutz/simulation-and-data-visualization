#include "volumerendererwidget.h"

#include <QFileInfo>

VolumeRendererWidget::VolumeRendererWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.volume_renderer, &VolumeRenderer::rendererReady, this, &VolumeRendererWidget::onRendererReady);
}

void VolumeRendererWidget::onRendererReady()
{
	auto path = QFileInfo(":/assets/volume/smallHeart.mhd").absoluteFilePath().toStdString();
	VolumeFileLoader::load(path, m_volumeData);
	
	ui.volume_renderer->setVolumeData(m_volumeData);
	
	m_histogram.update(m_volumeData.data, 4096);

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
