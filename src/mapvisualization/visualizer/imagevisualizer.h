#pragma once

#include <QColor>
#include <QImage>
#include <QPoint>

#include <vector>

struct ControlPoint
{
	QPoint pos;
	QColor color;
};

class ImageVisualizer
{
public:
	ImageVisualizer() = default;
	virtual ~ImageVisualizer() = default;

	virtual void paint(QImage& image, const std::vector<ControlPoint>& controlPoints) = 0;
};
