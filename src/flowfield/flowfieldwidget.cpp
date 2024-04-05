#include "flowfieldwidget.h"

FlowFieldWidget::FlowFieldWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setMap(0);
}

void FlowFieldWidget::changeFlowFieldWidth(int width)
{
	m_fieldWidth = width;
	m_cellField.initialize(m_imageRenderer.originalImage(), m_fieldWidth, m_fieldHeight);
	update();
}

void FlowFieldWidget::changeFlowFieldHeight(int height)
{
	m_fieldHeight = height;
	m_cellField.initialize(m_imageRenderer.originalImage(), m_fieldWidth, m_fieldHeight);
	update();
}

void FlowFieldWidget::showCells(bool show)
{
	m_showCells = show;
	update();
}

void FlowFieldWidget::setCellSize(int size)
{
	m_cellSize = size / 100.0f;
	update();
}

void FlowFieldWidget::setMap(int mapIndex)
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
			QColor color(cell.value, cell.value, cell.value);

			auto shapeWidth = pixelPerCellX * m_cellSize;
			auto shapeHeight = pixelPerCellY * m_cellSize;
			QRectF pointShape(-0.5f * shapeWidth, -0.5f * shapeHeight, shapeWidth, shapeHeight);

			QPen pen(Qt::GlobalColor::darkCyan);
			pen.setWidth(1);
			pen.setStyle(Qt::PenStyle::SolidLine);

			QBrush brush;
			brush.setStyle(Qt::BrushStyle::SolidPattern);
			brush.setColor(color);

			painter.setPen(pen);
			painter.setBrush(brush);
			painter.save();

			auto posX = (x * pixelPerCellX) + offsetX;
			auto posY = (y * pixelPerCellY) + offsetY;
			painter.translate(posX, posY);
			painter.drawEllipse(pointShape);
			painter.restore();
		}
	}
}
