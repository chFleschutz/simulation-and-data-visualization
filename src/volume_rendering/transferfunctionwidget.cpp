#include "transferfunctionwidget.h"

#include <QPainter>
#include <QMouseEvent>

TransferFunctionWidget::TransferFunctionWidget(QWidget *parent)
	: QWidget(parent)
{
	m_transferFunction.addControlPoint(0.0f, QColor::fromRgbF(0.0f, 0.0f, 0.0f));
	m_transferFunction.addControlPoint(1.0f, QColor::fromRgbF(1.0f, 1.0f, 1.0f));
}

void TransferFunctionWidget::setHistogram(const Histogram& histogram)
{
	const auto& histogramData = histogram.data();
	m_histogram = QImage(histogramData.size(), 256, QImage::Format_RGB32);
	m_histogram.fill(Qt::white);

	auto maxCount = histogram.maxCount();

	QPainter painter(&m_histogram);
	for (int x = 0; x < m_histogram.width(); x++)
	{
		auto count = histogramData[x];
		if (count == 0)
			continue;

		auto percent = count / static_cast<float>(maxCount);
		auto logPercent = std::log(count) / std::log(maxCount);

		Q_ASSERT(percent >= 0.0f && percent <= 1.0f);
		Q_ASSERT(logPercent >= 0.0f && logPercent <= 1.0f);

		int baseY = m_histogram.height();
		int linearY = baseY - (percent * m_histogram.height());
		int logY = baseY - (logPercent * m_histogram.height());

		painter.setPen(QPen(m_linearColor, 1));
		painter.drawLine(x, baseY, x, linearY);

		painter.setPen(QPen(m_logarithmicColor, 1));
		painter.drawLine(x, linearY, x, logY);
	}
}

void TransferFunctionWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	painter.drawImage(0, 0, m_histogram.scaled(size()));

	for (auto& [value, color] : m_transferFunction.controlPoints())
	{
		painter.setBrush(color);

		QPoint center(value * width(), height() / 2);
		painter.drawEllipse(center, 5, 5);
	}
}

void TransferFunctionWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	float t = event->pos().x() / static_cast<float>(width());

	m_transferFunction.addControlPoint(t, QColor(255, 0, 0));

	update();
}

void TransferFunctionWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() != Qt::RightButton)
		return;

	float t = event->pos().x() / static_cast<float>(width());

	auto color = m_transferFunction.color(t);
	qDebug() << "Color at" << t << ":" << color;
}
