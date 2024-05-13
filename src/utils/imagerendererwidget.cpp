#include "imagerendererwidget.h"

ImageRendererWidget::ImageRendererWidget(QWidget* parent)
	: QWidget(parent)
{
}

void ImageRendererWidget::updateSize()
{
	m_imageRenderer.resize(size());
	update();
}

void ImageRendererWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	m_imageRenderer.draw(painter);

	emit onPaintEvent(painter);
}

void ImageRendererWidget::resizeEvent(QResizeEvent* event)
{
	updateSize();

	emit onResized(event);
}

void ImageRendererWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	emit onDoubleClicked(event);
}

void ImageRendererWidget::mousePressEvent(QMouseEvent* event)
{
	emit onClicked(event);
}
