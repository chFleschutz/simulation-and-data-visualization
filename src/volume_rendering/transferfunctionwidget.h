#pragma once

#include "volume_rendering/transferfunction.h"
#include "volume_rendering/histogram.h"

#include <QWidget>
#include <QImage>

#include <vector>
#include <optional>

class TransferFunctionWidget : public QWidget
{
	Q_OBJECT

public:
	TransferFunctionWidget(QWidget* parent = nullptr);
	~TransferFunctionWidget() = default;

	void initialize();

	void setHistogram(const Histogram& histogram);

signals:
	void transferFunctionChanged(const QImage& transferFunction);

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);

private:
	void addControlPoint(float t, const QColor& color);
	void createTransferFunctionImage();

	int m_controlPointRadius = 7;
	std::optional<std::pair<float, QColor>> m_selectedKey;

	QImage m_histogram;

	TransferFunction m_transferFunction;
	QImage m_transferFunctionImage;
	int m_functionWidth = 4096;

	QColor m_linearColor = QColor(150, 150, 150, 200);
	QColor m_logarithmicColor = QColor(200, 200, 200, 200);
};
