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

	void setRadius(int radius)
	{
		m_radius = radius;
	}

	virtual void paint(QImage& image, const std::vector<ControlPoint>& controlPoints)
	{
		if (controlPoints.empty())
			return;

		for (int y = 0; y < image.height(); y++)
		{
			for (int x = 0; x < image.width(); x++)
			{
				if (image.pixelColor(x, y).alpha() == 0)
					continue;

				colorPixel(image, x, y, controlPoints);
			}
		}
	}

protected:
	virtual void colorPixel(QImage& image, int x, int y, const std::vector<ControlPoint>& controlPoints) = 0;

	int m_radius = 100;
};
