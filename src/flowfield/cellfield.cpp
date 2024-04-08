#include "cellfield.h"

#include <deque>

void CellField::initialize(QImage& image)
{
	Q_ASSERT_X(m_width > 0 && m_height > 0, "CellField::initialize", "Width and height must be greater than 0");
	initialize(image, m_width, m_height);
}

void CellField::initialize(QImage& image, uint32_t cellCountX, uint32_t cellCountY)
{
	m_width = cellCountX;
	m_height = cellCountY;

	m_field.resize(cellCountX);

	for (auto& column : m_field)
		column.resize(cellCountY);

	reset();
	setNeighbors();
	calcCostField(image);
}

void CellField::calcFlowField(uint32_t destX, uint32_t destY)
{
	m_destinationX = destX;
	m_destinationY = destY;

	reset();

	calcIntegrationField();
	calcFlowField();
}

void CellField::reset()
{
	for (auto& column : m_field)
	{
		for (auto& cell : column)
		{
			cell.integrationCost = 255;
			cell.flowDirection = QVector2D(0.0f, 0.0f);
		}
	}
}

void CellField::setNeighbors()
{
	std::array<int, 8> dx = { -1, 0, 1, -1, 1, -1, 0, 1 };
	std::array<int, 8> dy = { -1, -1, -1, 0, 0, 1, 1, 1 };

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			auto& cell = m_field[x][y];

			for (int i = 0; i < 8; i++)
			{
				auto neighborX = x + dx[i];
				auto neighborY = y + dy[i];

				Cell* neighbor = nullptr;
				if (neighborX >= 0 && neighborX < m_width && neighborY >= 0 && neighborY < m_height)
					neighbor = &m_field[neighborX][neighborY];

				cell.neighbors[i].cell = neighbor;
				cell.neighbors[i].direction = QVector2D(dx[i], dy[i]);
			}
		}
	}
}

void CellField::calcCostField(QImage& image)
{
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
					auto value = qRed(color); // Use red channel
					if (value < 250)		  // Use 255 as a barrier 
						value /= m_colorFactor;		  // Normalize to 0-5
					maxValue = std::max(maxValue, value); 
				}
			}

			auto& cell = m_field[cellX][cellY];
			cell.cost = maxValue;
			cell.integrationCost = 255;
		}
	}
}

void CellField::calcIntegrationField()
{
	// Breadth first search to calculate the cost to reach each cell from the start cell
	auto& startCell = m_field[m_destinationX][m_destinationY];
	startCell.integrationCost = 0;

	std::deque<Cell*> openList;
	openList.push_back(&startCell);

	while (!openList.empty())
	{
		auto currentCell = openList.front();
		openList.pop_front();

		for (auto& neighbor : currentCell->neighbors)
		{
			if (neighbor.cell == nullptr)
				continue;

			auto newCost = currentCell->integrationCost + neighbor.cell->cost;
			if (newCost < neighbor.cell->integrationCost)
			{
				neighbor.cell->integrationCost = newCost;
				openList.push_back(neighbor.cell);
			}
		}
	}
}

void CellField::calcFlowField()
{
	for (auto& column : m_field)
	{
		for (auto& cell : column)
		{
			int minCost = 255;
			Cell::Neighbor* bestNeighbor = nullptr;

			for (auto& neighbor : cell.neighbors)
			{
				if (neighbor.cell == nullptr)
					continue;

				if (neighbor.cell->integrationCost < minCost)
				{
					minCost = neighbor.cell->integrationCost;
					bestNeighbor = &neighbor;
				}
			}

			// No reachable neighbors found
			if (minCost == 255)
			{
				cell.flowDirection = QVector2D(0.0f, 0.0f);
				continue;
			}

			if (bestNeighbor != nullptr)
			{
				cell.flowDirection = bestNeighbor->direction;
			}
		}
	}

	auto& destinationCell = m_field[m_destinationX][m_destinationY];
	destinationCell.flowDirection = QVector2D(0.0f, 0.0f);
}
