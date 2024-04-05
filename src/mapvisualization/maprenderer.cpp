#include "maprenderer.h"

#include <QMouseEvent>
#include <QRandomGenerator>
#include <QFileDialog>

MapRenderer::MapRenderer(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	loadPreset(0);
}

void MapRenderer::reset()
{
	m_controlPoints.clear();
	resetImage();
}

void MapRenderer::loadImage()
{
	auto path = QFileDialog::getOpenFileName(this, "Open Image", QString(), "Images (*.png *.jpg *.bmp)");
	if (path.isEmpty())
		return;

	load(path);
}

void MapRenderer::loadPreset(int index)
{
	switch (index)
	{
	case 0:
		load(":/assets/images/DEsmall.png");
		break;
	case 1:
		load(":/assets/images/USA.png");
		break;
	case 2:
		load(":/assets/images/AUbig.png");
		break;
	default:
		break;
	}
}

void MapRenderer::setRadius(int radius)
{
	if (m_visualizer)
	{
		m_visualizer->setRadius(radius);
		resetImage();
	}
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
	QPainter painter(this);
	m_imageRenderer.draw(painter);
	drawPoints(painter);
}

void MapRenderer::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);

	if (size().height() == 0 || size().width() == 0)
		return;

	resetImage();
}

void MapRenderer::load(const QString& path)
{
	m_imageRenderer.load(path, size());
	// Reset the control points and image
	reset(); 
}

void MapRenderer::resetImage()
{
	m_imageRenderer.resize(size());
	updateImage(); // Trigger a repaint
}

void MapRenderer::updateImage()
{
	if (m_visualizer)
	{
		m_visualizer->paint(m_imageRenderer.image(), m_controlPoints);
	}

	update(); // Trigger a repaint
}

void MapRenderer::drawPoints(QPainter& painter)
{
	painter.setRenderHint(QPainter::Antialiasing);

	for (auto& point : m_controlPoints)
	{
		QRectF pointShape(-0.5f * m_pointSize, -0.5f * m_pointSize, m_pointSize, m_pointSize);

		QPen pen(Qt::black);
		pen.setWidth(1);
		pen.setStyle(Qt::PenStyle::SolidLine);

		QBrush brush;
		brush.setStyle(Qt::BrushStyle::SolidPattern);
		brush.setColor(point.color);

		painter.setPen(pen);
		painter.setBrush(brush);

		painter.save();
		painter.translate(point.pos);
		painter.drawEllipse(pointShape);
		painter.restore();
	}
}
