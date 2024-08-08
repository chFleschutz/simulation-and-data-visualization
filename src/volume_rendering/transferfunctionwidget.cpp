#include "transferfunctionwidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>

TransferFunctionWidget::TransferFunctionWidget(QWidget* parent)
	: QWidget(parent)
{
}

void TransferFunctionWidget::initialize()
{
	addControlPoint(0.15f, QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.02f));
	addControlPoint(0.4f, QColor::fromRgbF(1.0f, 1.0f, 1.0f, 0.98f));

	addControlPoint(0.22f, QColor(255, 85, 0, 200));
	addControlPoint(0.27f, QColor(255, 0, 0, 140));

	update();
}

void TransferFunctionWidget::setHistogram(const Histogram& histogram)
{
	const auto& histogramData = histogram.data();
	m_histogram = QImage(histogramData.size(), 256, QImage::Format_RGBA8888);
	m_histogram.fill(Qt::transparent);

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

	update();
}

void TransferFunctionWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	painter.drawImage(0, 0, m_transferFunctionImage.scaled(size()));
	painter.drawImage(0, 0, m_histogram.scaled(size()));

	for (auto& [value, color] : m_transferFunction.controlPoints())
	{
		painter.setBrush(color);

		QPoint center(value * width(), color.alphaF() * height());
		painter.drawEllipse(center, m_controlPointRadius, m_controlPointRadius);
	}
}

void TransferFunctionWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	QColor color = QColorDialog::getColor(Qt::white, this, "Select Color");
	if (!color.isValid())
		return;

	float t = event->pos().x() / static_cast<float>(width());
	t = std::clamp(t, 0.0f, 1.0f);

	float alpha = event->pos().y() / static_cast<float>(height());
	color.setAlphaF(std::clamp(alpha, 0.0f, 1.0f));

	addControlPoint(t, color);
	update();
}

void TransferFunctionWidget::mousePressEvent(QMouseEvent* event)
{
	// Left click to select control point
	if (event->button() == Qt::LeftButton)
	{
		for (auto& [value, color] : m_transferFunction.controlPoints())
		{
			QPoint center(value * width(), color.alphaF() * height());

			QRect controlPointRect(center.x() - m_controlPointRadius, center.y() - m_controlPointRadius, 2 * m_controlPointRadius, 2 * m_controlPointRadius);
			if (controlPointRect.contains(event->pos()))
			{
				m_selectedKey = { value, color };
				break;
			}
		}
	}

	// Right click to remove control point
	if (event->button() == Qt::RightButton)
	{
		for (auto& [value, color] : m_transferFunction.controlPoints())
		{
			QPoint center(value * width(), color.alphaF() * height());

			QRect controlPointRect(center.x() - m_controlPointRadius, center.y() - m_controlPointRadius, 2 * m_controlPointRadius, 2 * m_controlPointRadius);
			if (controlPointRect.contains(event->pos()))
			{
				m_transferFunction.erase(value);
				createTransferFunctionImage();
				update();
				break;
			}
		}
	}
}

void TransferFunctionWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (!m_selectedKey.has_value())
		return;

	float alpha = event->pos().y() / static_cast<float>(height());
	alpha = std::clamp(alpha, 0.0f, 1.0f);

	float t = event->pos().x() / static_cast<float>(width());
	t = std::clamp(t, 0.0f, 1.0f);

	// Don't allow duplicate keys
	if (m_selectedKey->first != t && m_transferFunction.contains(t))
		return;

	m_transferFunction.erase(m_selectedKey->first);
	m_selectedKey->first = t;
	m_selectedKey->second.setAlphaF(alpha);
	addControlPoint(m_selectedKey->first, m_selectedKey->second);

	qDebug() << "Moving control point to " << t << " with alpha " << alpha;

	update();
}

void TransferFunctionWidget::mouseReleaseEvent(QMouseEvent* event)
{
	m_selectedKey.reset();
}

void TransferFunctionWidget::addControlPoint(float t, const QColor& color)
{
	m_transferFunction.addControlPoint(t, color);
	createTransferFunctionImage();
}

void TransferFunctionWidget::createTransferFunctionImage()
{
	m_transferFunctionImage = QImage(m_functionWidth, 1, QImage::Format_RGBA8888);

	for (int x = 0; x < m_functionWidth; x++)
	{
		float t = x / static_cast<float>(m_functionWidth);
		auto color = m_transferFunction.color(t);

		m_transferFunctionImage.setPixelColor(x, 0, color);
	}

	emit transferFunctionChanged(m_transferFunctionImage);
}
