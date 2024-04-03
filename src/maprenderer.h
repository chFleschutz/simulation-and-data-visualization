#pragma once

#include "ui_maprenderer.h"

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QColor>

struct ControlPoint
{
	QPoint pos;
	QColor color;
};

class MapRenderer : public QWidget
{
	Q_OBJECT
public:
	MapRenderer(QWidget *parent = nullptr);
	~MapRenderer();

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
	void drawPoints();

	Ui::MapRendererClass ui;

	QPainter m_painter;
	QImage m_image;

	std::vector<ControlPoint> m_controlPoints;
	float m_pointSize = 10.0f;
};
