#pragma once

#include "ui_maprenderer.h"

#include "visualizer/imagevisualizer.h"

#include <QImage>
#include <QPainter>
#include <QWidget>

#include <memory>

class MapRenderer : public QWidget
{
	Q_OBJECT
public:
	MapRenderer(QWidget* parent = nullptr);
	~MapRenderer() = default;

	template <typename T, typename... Args>
	void setVisualizer(Args&&... args)
	{
		m_visualizer = std::make_unique<T>(std::forward<Args>(args)...);
		updateImage();
	}

	/// @brief Clears the control points and resets the image to the original image 
	void reset();

	/// @brief Opens a file dialog to load an image
	void loadImage();

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	/// @brief Colors the image using the visualizer
	void updateImage();

	/// @brief Draws all control points 
	void drawPoints();

	Ui::MapRendererClass ui;

	QImage m_originalImage; // Original unscaled image
	QImage m_image;			// Scaled and colored image

	QPainter m_painter;
	float m_pointSize = 10.0f;
	std::vector<ControlPoint> m_controlPoints;

	std::unique_ptr<ImageVisualizer> m_visualizer;
};
