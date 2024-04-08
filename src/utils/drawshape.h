#pragma once

#include <QPainter>

class DrawShape
{
public:
	static void Arrow(QPainter& painter, float size, float angle)
	{
		painter.save();

		painter.rotate(angle + 90.0f);
		
		QPointF points[3] = {
			QPointF(0, -0.5f * size),
			QPointF(0.25f * size, 0),
			QPointF(-0.25f * size, 0)
		};

		painter.drawPolygon(points, 3);
		painter.drawLine(QPointF(0, 0), QPointF(0, 0.5f * size));

		painter.restore();
	}
};