#include "cellfield.h"

void CellField::initialize(QImage& image, uint32_t cellCountX, uint32_t cellCountY)
{
	m_width = cellCountX;
	m_height = cellCountY;

	m_field.resize(cellCountX);

	for (auto& column : m_field)
		column.resize(cellCountY);

	auto pixelPerCellX = static_cast<float>(image.width()) / static_cast<float>(m_width);
	auto pixelPerCellY = static_cast<float>(image.height()) / static_cast<float>(m_height);

	for (uint32_t cellY = 0; cellY < m_height; cellY++)
	{
		for (uint32_t cellX = 0; cellX < m_width; cellX++)
		{
			int maxValue = 0;
			auto pixelOffsetX = cellX * pixelPerCellX;
			auto pixelOffsetY = cellY * pixelPerCellY;
			for (uint32_t pixelY = pixelOffsetY; pixelY < pixelOffsetY + pixelPerCellY; pixelY++)
			{
				for (uint32_t pixelX = pixelOffsetX; pixelX < pixelOffsetX + pixelPerCellX; pixelX++)
				{
					auto color = image.pixel(pixelX, pixelY);
					maxValue = std::max(maxValue, qRed(color)); // only use red channel
				}
			}

			auto& cell = m_field[cellX][cellY];
			cell.value = maxValue;
		}
	}
}
