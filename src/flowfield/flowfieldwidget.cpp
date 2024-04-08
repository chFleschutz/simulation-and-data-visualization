#include "flowfieldwidget.h"

#include "utils/drawshape.h"

#include <QMouseEvent>

FlowFieldWidget::FlowFieldWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	onSetMap(0);
}

void FlowFieldWidget::onChangeFlowFieldWidth(int width)
{
	m_fieldWidth = width;
	m_cellField.initialize(m_imageRenderer.originalImage(), m_fieldWidth, m_fieldHeight);
	update();
}

void FlowFieldWidget::onChangeFlowFieldHeight(int height)
{
	m_fieldHeight = height;
	m_cellField.initialize(m_imageRenderer.originalImage(), m_fieldWidth, m_fieldHeight);
	update();
}

void FlowFieldWidget::onShowCells(bool show)
{
	m_showCells = show;
	update();
}

void FlowFieldWidget::onSetCellSize(int size)
{
	m_cellSize = size / 100.0f;
	update();
}

void FlowFieldWidget::onSetMap(int mapIndex)
{
	switch (mapIndex)
	{
	case 0:
		m_imageRenderer.load(":/assets/images/FlowfieldMap.png", size());
		break;
	default:
		break;
	}

	m_cellField.initialize(m_imageRenderer.originalImage(), m_fieldWidth, m_fieldHeight);
	update();
}

void FlowFieldWidget::onShowIntegrationCosts()
{
	m_showCosts = INTEGRATION_COSTS;
	update();
}

void FlowFieldWidget::onShowFlowDirection()
{
	m_showCosts = FLOW_DIRECTION;
	update();
}

void FlowFieldWidget::onShowTravelCosts()
{
	m_showCosts = TRAVEL_COSTS;
	update();
}

void FlowFieldWidget::onHideCosts()
{
	m_showCosts = STATS_HIDDEN;
	update();
}

void FlowFieldWidget::onResetField()
{
	m_cellField.reset();
	update();
}

void FlowFieldWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	m_imageRenderer.draw(painter);

	if (m_showCells)
		drawCellValues(painter);
}

void FlowFieldWidget::resizeEvent(QResizeEvent* event)
{
	m_imageRenderer.resize(size());
	update();
}

void FlowFieldWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	auto pos = event->pos();
	auto percentX = pos.x() / static_cast<float>(m_imageRenderer.size().width());
	auto percentY = pos.y() / static_cast<float>(m_imageRenderer.size().height());

	if (percentX < 0.0f || percentX > 1.0f || percentY < 0.0f || percentY > 1.0f)
		return;

	auto cellX = static_cast<uint32_t>(percentX * m_cellField.width());
	auto cellY = static_cast<uint32_t>(percentY * m_cellField.height());
	m_cellField.calcFlowField(cellX, cellY);
	update();
}

void FlowFieldWidget::drawCellValues(QPainter& painter)
{
	painter.setRenderHint(QPainter::Antialiasing, true);

	auto pixelPerCellX = static_cast<float>(m_imageRenderer.size().width()) / static_cast<float>(m_cellField.width());
	auto pixelPerCellY = static_cast<float>(m_imageRenderer.size().height()) / static_cast<float>(m_cellField.height());
	auto offsetX = pixelPerCellX / 2.0f;
	auto offsetY = pixelPerCellY / 2.0f;

	for (uint32_t y = 0; y < m_cellField.height(); y++)
	{
		for (uint32_t x = 0; x < m_cellField.width(); x++)
		{
			auto& cell = m_cellField.cell(x, y);
			QColor color(cell.cost, cell.cost, cell.cost);

			auto shapeWidth = pixelPerCellX * m_cellSize;
			auto shapeHeight = pixelPerCellY * m_cellSize;
			QRectF pointShape(-0.5f * shapeWidth, -0.5f * shapeHeight, shapeWidth, shapeHeight);

			QPen pen(Qt::GlobalColor::green);
			pen.setWidth(1);
			pen.setStyle(Qt::PenStyle::SolidLine);
			painter.setPen(pen);

			QBrush brush;
			brush.setStyle(Qt::BrushStyle::SolidPattern);
			brush.setColor(color);
			//painter.setBrush(brush);
			painter.save();

			auto posX = (x * pixelPerCellX) + offsetX;
			auto posY = (y * pixelPerCellY) + offsetY;
			painter.translate(posX, posY);
			painter.drawEllipse(pointShape);
			painter.restore();

			auto size = std::min(pixelPerCellX, pixelPerCellY);
			drawCosts(painter, cell, size, posX, posY);
		}
	}
}

void FlowFieldWidget::drawCosts(QPainter& painter, CellField::Cell& cell, float cellSize, float posX, float posY)
{
	uint32_t displayValue = 0;
	switch (m_showCosts)
	{
	case FlowFieldWidget::TRAVEL_COSTS:
		displayValue = cell.cost;
		break;
	case FlowFieldWidget::INTEGRATION_COSTS:
		displayValue = cell.integrationCost;
		break;
	case FlowFieldWidget::FLOW_DIRECTION:
		if (cell.flowDirection.length() > 0.0f)
		{
			painter.translate(posX, posY);
			DrawShape::Arrow(painter, 0.7f * cellSize, toAngle(cell.flowDirection));
			painter.translate(-posX, -posY);
		}
		return;
	case FlowFieldWidget::STATS_HIDDEN:
		// Return early if costs are hidden
		return;
	default:
		break;
	}

	auto shift = 2.0f * digits(displayValue);
	painter.setPen(Qt::GlobalColor::green);
	painter.drawText(QPointF(posX - shift, posY + 4), QString::number(displayValue));
}

int FlowFieldWidget::digits(int x, int base)
{
	int digits = 1;
	while (x > 0)
	{
		x /= base;
		digits++;
	}
	return digits;
}

float FlowFieldWidget::toAngle(const QVector2D& direction)
{
	return std::atan2(direction.y(), direction.x()) * 180.0f / static_cast<float>(M_PI);
}
