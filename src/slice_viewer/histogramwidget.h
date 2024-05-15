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

	void load(const Histogram& histogram);

	bool showWindowing() const { return m_windowEnabled; }
	/// @brief Returns the window level as a fraction [0, 1].
	float windowLevel() const { return m_windowLevel; }
	/// @brief Returns the window width as a fraction [0, 1].
	float windowWidth() const { return m_windowWidth; }

signals:
	void windowingChanged(bool enable);
	void windowLevelChanged(float level);
	void windowWidthChanged(float width);

public slots:
	void setShowWindowing(bool enable);
	/// @brief Set the window level as a percentage [0, 100].
	void setWindowLevel(int level);
	/// @brief Set the window width as a percentage [0, 100].
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

	bool m_windowEnabled = false;
	float m_windowLevel = 0.5f;
	float m_windowWidth = 1.0f;

	QColor m_linearColor = QColor(150, 150, 150);
	QColor m_logarithmicColor = QColor(200, 200, 200);

	bool m_dragging = false;
};
