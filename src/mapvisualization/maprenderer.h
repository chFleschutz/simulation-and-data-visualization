#pragma once

#include "ui_maprenderer.h"

#include "visualizer/imagevisualizer.h"

#include <QImage>
#include <QPainter>
#include <QWidget>

#include <memory>

#include "utils/imagerenderer.h"

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
		resetImage();
	}

public slots:
	/// @brief Clears the control points, resets the image and triggers an update
	void reset();

	/// @brief Opens a file dialog to load an image
	void loadImage();

	/// @brief Loads a preset image
	void loadPreset(int index);

	/// @brief Sets the radius used by the Shepard-Frankie-Little visualizer
	void setRadius(int radius);

	/// @brief Colors the image using the visualizer
	void updateImage();

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	/// @brief Loads an image from a file
	void load(const QString& path);

	/// @brief Resets the image to the original image and triggers an update
	void resetImage();

	/// @brief Draws all control points 
	void drawPoints(QPainter& painter);

	Ui::MapRendererClass ui;

	ImageRenderer m_imageRenderer;

	float m_pointSize = 10.0f;
	std::vector<ControlPoint> m_controlPoints;

	std::unique_ptr<ImageVisualizer> m_visualizer;
};
