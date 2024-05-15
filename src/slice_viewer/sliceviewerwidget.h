#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <memory>

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

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	void setupShaders();
	void setupGeometry();
	void setupTexture();
	void checkOpenGLError(const char* file, int line);

	QOpenGLFunctions* m_glFunctions = nullptr;

	QOpenGLShaderProgram m_program;
	QOpenGLBuffer m_vbo;
	QOpenGLVertexArrayObject m_vao;

	QOpenGLTexture m_texture = QOpenGLTexture(QOpenGLTexture::Target2D);
};
