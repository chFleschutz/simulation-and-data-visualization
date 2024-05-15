#pragma once

#include "imagerenderer.h"

#include <QWidget>
#include <QMouseEvent>

class ImageRendererWidget : public QWidget
{
	Q_OBJECT

public:
	ImageRendererWidget(QWidget* parent = nullptr);
	~ImageRendererWidget() = default;

	ImageRenderer& renderer() { return m_imageRenderer; }

	int imageWidth() const { return m_imageRenderer.width(); }
	int imageHeight() const { return m_imageRenderer.height(); }

	void updateSize();

signals:
	void onPaintEvent(QPainter& painter);
	void onDoubleClicked(QMouseEvent* event);
	void onClicked(QMouseEvent* event);
	void onResized(QResizeEvent* event);

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

private:
	ImageRenderer m_imageRenderer;
};
