#include "maprenderer.h"

#include <QMouseEvent>
#include <QRandomGenerator>
#include <QFileDialog>

MapRenderer::MapRenderer(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setContentsMargins(0, 0, 0, 0);
	setMinimumSize(512, 512);

	if (!m_originalImage.load("assets/DEsmall.png"))
		qFatal() << "Could not find image: " << "assets/DEsmall.png";

	reset();
}

void MapRenderer::reset()
{
	m_controlPoints.clear();

	m_image = m_originalImage.scaled(size(), Qt::AspectRatioMode::KeepAspectRatio);
	updateImage();
}

void MapRenderer::loadImage()
{
	auto path = QFileDialog::getOpenFileName(this, "Open Image", QString(), "Images (*.png *.jpg *.bmp)");
	if (path.isEmpty())
		return;

	if (!m_originalImage.load(path))
		qFatal() << "Could not find image: " << path;

	reset();
}

void MapRenderer::mouseDoubleClickEvent(QMouseEvent* event)
{
	auto pos = event->position().toPoint();

	auto r = QRandomGenerator::global()->generate() % 256;
	auto g = QRandomGenerator::global()->generate() % 256;
	auto b = QRandomGenerator::global()->generate() % 256;

	m_controlPoints.emplace_back(pos, QColor(r, g, b));

	updateImage();
}

void MapRenderer::paintEvent(QPaintEvent* event)
{
	m_painter.begin(this);

	m_painter.drawImage(0, 0, m_image);
	drawPoints();

	m_painter.end();
}

void MapRenderer::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);

	if (size().height() == 0 || size().width() == 0)
		return;

	m_image = m_originalImage.scaled(size(), Qt::AspectRatioMode::KeepAspectRatio);

	updateImage();
}

void MapRenderer::updateImage()
{
	if (m_visualizer)
	{
		m_visualizer->paint(m_image, m_controlPoints);
	}

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
