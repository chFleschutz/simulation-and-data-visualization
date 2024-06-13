#include "volumerendererwidget.h"

VolumeRendererWidget::VolumeRendererWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
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
