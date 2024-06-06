#pragma once

#include "slice_viewer/volumedatamanager.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>

#include <memory>

class VolumeRenderer : public QOpenGLWidget
{
	Q_OBJECT

public:
	enum RenderMode
	{
		EntryPoints = 0,
		ExitPoints = 1,
		Texture = 2,
		VolumeMIP = 3,
		VolumeDRR = 4
	};

	VolumeRenderer(QWidget* parent = nullptr);
	~VolumeRenderer();

	void setRenderMode(RenderMode mode) { m_renderMode = mode; }

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

private:
	void createExitPointFBO(int width, int height);
	void loadVolumeData();
	void setupVolumeTexture();
	void setupShaders();
	void setupGeometry();
	QMatrix4x4 createViewMatrix() const;

	void backFacePass();
	void volumePass();

	std::unique_ptr<QOpenGLFramebufferObject> m_exitPointFBO;

	QOpenGLShaderProgram m_exitPointShader;
	QOpenGLShaderProgram m_raycastShader;

	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	QOpenGLBuffer m_ibo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

	VolumeDataManager m_volumeData;
	QOpenGLTexture m_volumeTexture = QOpenGLTexture(QOpenGLTexture::Target3D);

	QMatrix4x4 m_model;
	QMatrix4x4 m_view;
	QMatrix4x4 m_projection;
	float m_stepSize = 0.001f;
	QVector3D m_volumeSize;

	QVector3D cameraPosition = QVector3D(0.0f, 0.0f, 3.0f);
	QVector3D cameraRotation = QVector3D(0.0f, 0.0f, 0.0f);

	float m_mouseSensitivity = 0.25f;
	float m_zoomSensitivity = 0.005f;
	float m_minZoom = 1.0f;
	float m_maxZoom = 10.0f;

	bool m_mousePressed = false;
	QPoint m_lastMousePos;

	RenderMode m_renderMode = RenderMode::VolumeMIP;
};