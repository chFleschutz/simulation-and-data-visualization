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

void VolumeRendererWidget::onRenderModeExitPoints()
{
	ui.volume_renderer->setRenderMode(VolumeRenderer::RenderMode::ExitPoints);
}

void VolumeRendererWidget::onRenderModeTexture()
{
	ui.volume_renderer->setRenderMode(VolumeRenderer::RenderMode::Texture);
}

void VolumeRendererWidget::onRenderModeVolume()
{
	ui.volume_renderer->setRenderMode(VolumeRenderer::RenderMode::Volume);
}
