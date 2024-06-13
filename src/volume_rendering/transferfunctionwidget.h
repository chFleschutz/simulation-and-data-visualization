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

	void initialize();

	void setHistogram(const Histogram& histogram);

signals:
	void transferFunctionChanged(const QImage& transferFunction);

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);

private:
	void createTransferFunctionImage();

	QImage m_histogram;

	TransferFunction m_transferFunction;
	QImage m_transferFunctionImage;
	int m_functionWidth = 256;

	QColor m_linearColor = QColor(150, 150, 150, 200);
	QColor m_logarithmicColor = QColor(200, 200, 200, 200);
};
