#pragma once

#include "imagevisualizer.h"

class Shepard : public ImageVisualizer
{
public:
	Shepard() = default;
	virtual ~Shepard() = default;

protected:
	void colorPixel(QImage& image, int x, int y, const std::vector<ControlPoint>& controlPoints) override
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float totalWeight = 0.0f;

		for (auto& point : controlPoints)
		{
			auto dx = point.pos.x() - x;
			auto dy = point.pos.y() - y;
			auto dist = sqrt(dx * dx + dy * dy); 

			auto weight = 1.0f / dist;
			totalWeight += weight;

			r += weight * point.color.red();
			g += weight * point.color.green();
			b += weight * point.color.blue();
		}

		r /= totalWeight;
		g /= totalWeight;
		b /= totalWeight;

		image.setPixelColor(x, y, QColor(r, g, b));
	}
};