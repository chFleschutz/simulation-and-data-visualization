#include "maprenderer.h"

#include <QMouseEvent>
#include <QRandomGenerator>

MapRenderer::MapRenderer(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QString imagePath = "assets/DEsmall.png";
	if (!m_image.load(imagePath))
		qFatal() << "Could not find image: " << imagePath;
}

MapRenderer::~MapRenderer()
{
}

void MapRenderer::paintEvent(QPaintEvent* event)
{
	m_painter.begin(this);

	m_painter.drawImage(0, 0, m_image.scaledToHeight(height()));
	drawPoints();

	m_painter.end();
}

void MapRenderer::mouseDoubleClickEvent(QMouseEvent* event)
{
	auto pos = event->position().toPoint();

	auto r = QRandomGenerator::global()->generate() % 256;
	auto g = QRandomGenerator::global()->generate() % 256;
	auto b = QRandomGenerator::global()->generate() % 256;

	m_controlPoints.emplace_back(pos, QColor(r, g, b));

	update(); // Trigger a repaint
}

void MapRenderer::drawPoints()
{
	m_painter.setRenderHint(QPainter::Antialiasing);

	for (auto& point : m_controlPoints)
	{
		QRectF pointShape(-0.5f * m_pointSize, -0.5f * m_pointSize, m_pointSize, m_pointSize);

		QPen pen(Qt::black);
		pen.setWidth(1);
		pen.setStyle(Qt::PenStyle::SolidLine);

		QBrush brush;
		brush.setStyle(Qt::BrushStyle::SolidPattern);
		brush.setColor(point.color);

		m_painter.setPen(pen);
		m_painter.setBrush(brush);

		m_painter.save();
		m_painter.translate(point.pos);
		m_painter.drawEllipse(pointShape);
		m_painter.restore();
	}
}
