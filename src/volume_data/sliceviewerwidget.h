#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

class SliceViewerWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	SliceViewerWidget(QWidget* parent = 0);
	~SliceViewerWidget();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	void setupShaders();
	void setupGeometry();
	void checkOpenGLError(const char* file, int line);

	QOpenGLFunctions* m_glFunctions = nullptr;

	QOpenGLShaderProgram m_program;
	QOpenGLBuffer m_vbo;
	QOpenGLVertexArrayObject m_vao;

	QMatrix4x4 m_projectionMatrix;
};
