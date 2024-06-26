#pragma once

#include "imagevisualizer.h"

#include <limits>

class Voronoi : public ImageVisualizer
{
public:
	Voronoi() = default;
	virtual ~Voronoi() = default;

protected:
	void colorPixel(QImage& image, int x, int y, const std::vector<ControlPoint>& controlPoints)
	{
		float minDist = std::numeric_limits<float>::max();
		QPoint pixelPos(x, y);
		for (auto& point : controlPoints)
		{
			auto dx = point.pos.x() - x;
			auto dy = point.pos.y() - y;
			auto dist = dx * dx + dy * dy; // Use distance squared to avoid sqrt

			if (dist < minDist)
			{
				minDist = dist;
				image.setPixelColor(pixelPos, point.color);
			}
		}
	}
};
