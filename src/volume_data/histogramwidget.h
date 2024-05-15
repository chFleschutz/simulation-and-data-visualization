#pragma once

#include <QWidget>

#include "utils/imagerenderer.h"
#include "volumedatamanager.h"

class HistogramWidget : public QWidget
{
	Q_OBJECT

public:
	HistogramWidget(QWidget *parent = nullptr);
	~HistogramWidget() = default;

	void load(const Histogram<uint16_t>& histogram);

	bool showWindowing() const { return m_showWindow; }
	float windowLevel() const { return m_windowLevel; }
	float windowWidth() const { return m_windowWidth; }

signals:
	void windowLevelChanged(float level);
	void windowWidthChanged(float width);

public slots:
	void setShowWindowing(bool enable);
	void setWindowLevel(int level);
	void setWindowWidth(int width);

protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void resizeEvent(QResizeEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
	virtual void wheelEvent(QWheelEvent *event) override;
	
private:
	ImageRenderer m_image;

	bool m_showWindow = false;
	float m_windowLevel = 0.5f;
	float m_windowWidth = 1.0f;

	QColor m_linearColor = QColor(150, 150, 150);
	QColor m_logarithmicColor = QColor(200, 200, 200);

	bool m_dragging = false;
};
