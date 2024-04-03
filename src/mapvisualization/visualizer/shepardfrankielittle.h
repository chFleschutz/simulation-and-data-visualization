#pragma once

#include "imagevisualizer.h"

#include <algorithm>

class ShepardFrankieLittle : public ImageVisualizer
{
public:
	ShepardFrankieLittle() = default;
	virtual ~ShepardFrankieLittle() = default;

protected:
	void colorPixel(QImage& image, int x, int y, const std::vector<ControlPoint>& controlPoints)
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float totalWeight = 0.0f;

		for (auto& point : controlPoints)
		{
			auto dx = point.pos.x() - x;
			auto dy = point.pos.y() - y;
			float dist = sqrt(dx * dx + dy * dy);

			auto weight = std::max(m_radius - dist, 0.0f) / (m_radius * dist);
			totalWeight += weight;

			r += weight * point.color.red();
			g += weight * point.color.green();
			b += weight * point.color.blue();
		}

		// Avoid division by zero (or near zero)
		if (totalWeight < 0.0001f)
			return;

		r /= totalWeight;
		g /= totalWeight;
		b /= totalWeight;

		image.setPixelColor(x, y, QColor(r, g, b));
	}
};