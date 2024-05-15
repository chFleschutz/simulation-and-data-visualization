#include "histogramwidget.h"

#include <QPainter>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QWheelEvent>

HistogramWidget::HistogramWidget(QWidget* parent)
	: QWidget(parent)
{
	m_image.setAspectRatioMode(Qt::IgnoreAspectRatio);
	m_image.load(":/assets/images/AUbig.png");
}

void HistogramWidget::load(const Histogram& histogram)
{
	m_image.create(QSize(1024, 512));
	auto& image = m_image.originalImage();
	image.fill(Qt::white);

	QPainter painter(&image);

	auto maxValue = histogram.maxValue;
	auto maxCount = histogram.maxCount;

	for (auto [key, value] : histogram.data)
	{
		auto percentX = static_cast<float>(key) / static_cast<float>(maxValue);
		auto percentY = static_cast<float>(value) / static_cast<float>(maxCount);
		auto logPercentY = std::log(value) / std::log(maxCount);

		auto x = percentX * image.width();
		auto linearY = percentY * image.height();
		auto logY = logPercentY * image.height();

		painter.setPen(QPen(m_linearColor, 1));
		painter.drawLine(x, image.height(), x, image.height() - linearY);

		painter.setPen(QPen(m_logarithmicColor, 1));
		painter.drawLine(x, image.height() - linearY, x, image.height() - logY);
	}

	m_image.resize(size());
	update();
}

void HistogramWidget::setShowWindowing(bool enable)
{
	m_showWindow = enable;
	update();
}

void HistogramWidget::setWindowLevel(int level)
{
	m_windowLevel = level / 100.0f;
	update();
}

void HistogramWidget::setWindowWidth(int width)
{
	m_windowWidth = width / 100.0f;
	update();
}

void HistogramWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	// Draw histogram
	m_image.draw(painter);

	// Draw windowing
	if (m_showWindow)
	{
		auto windowCenterX = m_image.width() * m_windowLevel;
		auto windowWidth = m_image.width() * m_windowWidth;
		painter.setPen(Qt::transparent); 
		painter.setBrush(QBrush(QColor(0, 200, 0, 100)));
		painter.drawRect(windowCenterX - (windowWidth * 0.5f), 0, windowWidth, m_image.height());
	}
}

void HistogramWidget::resizeEvent(QResizeEvent* event)
{
	m_image.resize(event->size());
	update();
}

void HistogramWidget::mousePressEvent(QMouseEvent* event)
{
	m_dragging = true;
	m_windowLevel = static_cast<float>(event->pos().x()) / static_cast<float>(m_image.width());
	emit windowLevelChanged(m_windowLevel);
	update();
}

void HistogramWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (m_dragging)
	{
		m_windowLevel = static_cast<float>(event->pos().x()) / static_cast<float>(m_image.width());
		emit windowLevelChanged(m_windowLevel);
		update();
	}
}

void HistogramWidget::mouseReleaseEvent(QMouseEvent* event)
{
	m_dragging = false;
}

void HistogramWidget::wheelEvent(QWheelEvent* event)
{
	auto delta = event->angleDelta().y();
	if (delta > 0)
	{
		m_windowWidth = std::min(1.0f, m_windowWidth + 0.05f);
	}
	else
	{
		m_windowWidth = std::max(0.0f, m_windowWidth - 0.05f);
	}

	emit windowWidthChanged(m_windowWidth);
	update();
}
