#pragma once

#include "volume_rendering/transferfunction.h"
#include "volume_rendering/histogram.h"

#include <QWidget>

#include <vector>

class TransferFunctionWidget : public QWidget
{
	Q_OBJECT

public:
	TransferFunctionWidget(QWidget* parent = nullptr);
	~TransferFunctionWidget() = default;

	void setHistogram(const Histogram& histogram);

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);

private:
	QImage m_histogram;
	TransferFunction m_transferFunction;

	QColor m_linearColor = QColor(150, 150, 150);
	QColor m_logarithmicColor = QColor(200, 200, 200);
};
