#pragma once

#include "utils/utils.h"

#include <QColor>

#include <map>

class TransferFunction
{
public:
	TransferFunction() = default;

	// Adds a control point at the given value with the given color
	void addControlPoint(float value, const QColor& color)
	{
		m_controlPoints[value] = color;
	}

	// Returns the color for the given value
	QColor color(float value) const
	{
		Q_ASSERT_X(!m_controlPoints.empty(), "TransferFunction::color", "No control points added yet");

		auto upper = m_controlPoints.lower_bound(value);

		// Value smaller than smallest control point value -> return smallest control point color
		if (upper == m_controlPoints.begin())
			return m_controlPoints.begin()->second;

		// Value larger than largest control point value -> return largest control point color
		if (upper == m_controlPoints.end())
			return m_controlPoints.rbegin()->second;

		auto lower = std::prev(upper);
		const auto& [lowerVal, lowerColor] = *lower;
		const auto& [upperVal, upperColor] = *upper;

		// interpolate between the two bounds
		float t = (value - lowerVal) / (upperVal - lowerVal);
		return Utils::lerp(lowerColor, upperColor, t);
	}

	const std::map<float, QColor>& controlPoints() const { return m_controlPoints; }

private:
	std::map<float, QColor> m_controlPoints;
};