#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class VolumeRenderer : public QOpenGLWidget
{
	Q_OBJECT

public:
	VolumeRenderer(QWidget* parent = nullptr);
	~VolumeRenderer();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

private:
	void setupShaders();
	void setupGeometry();
	QMatrix4x4 createViewMatrix() const;

	QOpenGLShaderProgram m_program;
	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	QOpenGLBuffer m_ibo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

	QMatrix4x4 m_model;
	QMatrix4x4 m_view;
	QMatrix4x4 m_projection;

	QVector3D cameraPosition = QVector3D(0.0f, 0.0f, 3.0f);
	QVector3D cameraRotation = QVector3D(0.0f, 0.0f, 0.0f);

	float m_mouseSensitivity = 0.25f;
	float m_zoomSensitivity = 0.005f;

	bool m_mousePressed = false;
	QPoint m_lastMousePos;
};