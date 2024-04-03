#pragma once

#include "imagevisualizer.h"

class Shepard : public ImageVisualizer
{
public:
	Shepard() = default;
	virtual ~Shepard() = default;

	virtual void paint(QImage& image, const std::vector<ControlPoint>& controlPoints) override
	{

	}

};