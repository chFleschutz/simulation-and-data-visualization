#pragma once

#include <QColor>

class Utils
{
public:
	static float lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	static QColor lerp(const QColor& a, const QColor& b, float t)
	{
		return QColor::fromRgbF(
			lerp(a.redF(), b.redF(), t),
			lerp(a.greenF(), b.greenF(), t),
			lerp(a.blueF(), b.blueF(), t),
			lerp(a.alphaF(), b.alphaF(), t)
		);
	}
};