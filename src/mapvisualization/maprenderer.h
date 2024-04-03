#pragma once

#include "ui_maprenderer.h"

#include "visualizer/imagevisualizer.h"

#include <QImage>
#include <QPainter>
#include <QWidget>

#include <memory>

class MapRenderer : public QWidget
{
	Q_OBJECT
public:
	MapRenderer(QWidget* parent = nullptr);
	~MapRenderer() = default;

	template <typename T, typename... Args>
	void setVisualizer(Args&&... args)
	{
		m_visualizer = std::make_unique<T>(std::forward<Args>(args)...);
		updateImage();
	}

	void reset();

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPaintEvent* event) override;

private:
	void loadImage();
	void updateImage();
	void drawPoints();

	Ui::MapRendererClass ui;

	QString m_imagePath = "assets/DEsmall.png";
	QImage m_image;
	QPainter m_painter;

	float m_pointSize = 10.0f;
	std::vector<ControlPoint> m_controlPoints;

	std::unique_ptr<ImageVisualizer> m_visualizer;
};
