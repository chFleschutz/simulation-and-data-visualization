#pragma once

#include <QImage>
#include <QVector2D>

#include <vector>
#include <array>

class CellField
{
public:
	struct Cell
	{
		struct Neighbor
		{
			Cell* cell = nullptr;
			QVector2D direction;
		};

		uint32_t cost;
		uint32_t integrationCost;
		QVector2D flowDirection;

		std::array<Neighbor, 8> neighbors;
	};

	CellField() = default;
	~CellField() = default;

	void initialize(QImage& image);
	void initialize(QImage& image, uint32_t cellCountX, uint32_t cellCountY);

	void calcFlowField(uint32_t destX, uint32_t destY);

	void reset();

	uint32_t width() const { return m_width; }
	uint32_t height() const { return m_height; }

	Cell& cell(uint32_t x, uint32_t y) { return m_field[x][y]; }

	float colorFactor() const { return m_colorFactor; }

private:
	void setNeighbors();
	void calcCostField(QImage& image);
	void calcIntegrationField();
	void calcFlowField();

	uint32_t m_width = 1;
	uint32_t m_height = 1;
	std::vector<std::vector<Cell>> m_field;

	uint32_t m_destinationX = 0;
	uint32_t m_destinationY = 0;

	float m_colorFactor = 50.0f;
};