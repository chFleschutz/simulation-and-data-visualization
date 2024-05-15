#pragma once

#include "slice_viewer/volumedata.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class SliceViewerWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	struct Vertex
	{
		QVector3D position;
		QVector2D texCoord;
	};

	SliceViewerWidget(QWidget* parent = 0);
	~SliceViewerWidget();

	void load(const VolumeData& data);

	void setWindowLevel(float level);
	void setWindowWidth(float width);
	void setWindowing(bool enabled);

signals:
	void sliceLevelChanged(float level);

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	virtual void wheelEvent(QWheelEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	void setupShaders();
	void setupGeometry();
	void setupTexture();
	void checkOpenGLError(const char* file, int line);

	QOpenGLFunctions* m_glFunctions = nullptr;

	QOpenGLShaderProgram m_program;
	QOpenGLBuffer m_vbo;
	QOpenGLVertexArrayObject m_vao;

	QOpenGLTexture m_texture = QOpenGLTexture(QOpenGLTexture::Target3D);

	float m_sliceLevel = 0.0f;
	float m_windowWidth = 1.0f;
	float m_windowLevel = 0.5f;
	bool m_windowing = false;
};
