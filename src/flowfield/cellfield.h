#pragma once

#include <QImage>

#include <vector>

struct Cell
{
	uint32_t value;
};

class CellField
{
public:
	CellField() = default;
	~CellField() = default;

	void initialize(QImage& image, uint32_t cellCountX, uint32_t cellCountY);

	uint32_t width() const { return m_width; }
	uint32_t height() const { return m_height; }

	Cell& cell(uint32_t x, uint32_t y) { return m_field[x][y]; }

private:
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	std::vector<std::vector<Cell>> m_field;
};