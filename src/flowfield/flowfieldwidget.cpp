#include "flowfieldwidget.h"

FlowFieldWidget::FlowFieldWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	if (!m_originalMap.load(":/assets/images/FlowfieldMap.png"))
		qFatal() << "Failed to load image";

	m_map = m_originalMap.scaled(size(), Qt::AspectRatioMode::KeepAspectRatio);

	m_cellField.initialize(m_map, 50, 50);
}

FlowFieldWidget::~FlowFieldWidget()
{}

void FlowFieldWidget::changeFlowFieldWidth(int width)
{
	m_cellField.initialize(m_originalMap, width, m_cellField.height());
	update();
}


void FlowFieldWidget::changeFlowFieldHeight(int height)
{
	m_cellField.initialize(m_originalMap, m_cellField.width(), height);
	update();
}

void FlowFieldWidget::paintEvent(QPaintEvent * event)
{
	m_painter.begin(this);
	m_painter.drawImage(0, 0, m_map);
	drawCellValues();
	m_painter.end();
}

void FlowFieldWidget::resizeEvent(QResizeEvent* event)
{
	m_map = m_originalMap.scaled(size(), Qt::AspectRatioMode::KeepAspectRatio);
	m_cellField.initialize(m_originalMap, m_cellField.width(), m_cellField.height());
}

void FlowFieldWidget::drawCellValues()
{
	auto pixelPerCellX = static_cast<float>(m_map.width()) / static_cast<float>(m_cellField.width());
	auto pixelPerCellY = static_cast<float>(m_map.height()) / static_cast<float>(m_cellField.height());
	auto offsetX = pixelPerCellX / 2.0f;
	auto offsetY = pixelPerCellY / 2.0f;

	for (uint32_t y = 0; y < m_cellField.height(); y++)
	{
		for (uint32_t x = 0; x < m_cellField.width(); x++)
		{
			auto& cell = m_cellField.cell(x, y);
			QColor color(cell.value, cell.value, cell.value);

			QRectF pointShape(-5.0f, -5.0f, 5.0f, 5.0f);

			QPen pen(Qt::blue);
			pen.setWidth(1);
			pen.setStyle(Qt::PenStyle::SolidLine);

			QBrush brush;
			brush.setStyle(Qt::BrushStyle::SolidPattern);
			brush.setColor(color);

			m_painter.setPen(pen);
			m_painter.setBrush(brush);

			m_painter.save();

			auto posX = (x * pixelPerCellX) + offsetX;
			auto posY = (y * pixelPerCellY) + offsetY;
			m_painter.translate(posX, posY);

			m_painter.drawEllipse(pointShape);
			m_painter.restore();
		}
	}
}
