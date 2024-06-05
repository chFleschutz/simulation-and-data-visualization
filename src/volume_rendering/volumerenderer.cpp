#include "volumerenderer.h"

#include "utils/openglutils.h"

#include <QMouseEvent>

VolumeRenderer::VolumeRenderer(QWidget* parent)
	: QOpenGLWidget(parent)
{
}

VolumeRenderer::~VolumeRenderer()
{
	makeCurrent();

	m_vbo.destroy();
	m_ibo.destroy();
	m_vao.destroy();

	doneCurrent();
}

void VolumeRenderer::initializeGL()
{
	auto glFunctions = OpenGLUtils::initializeOpenGLFunctions();
	glFunctions->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	setupShaders();
	setupGeometry();

	m_view.lookAt(cameraPosition, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));

	CHECK_GL_ERROR();
}

void VolumeRenderer::resizeGL(int w, int h)
{
	auto glFunctions = QOpenGLContext::currentContext()->functions();
	glFunctions->glViewport(0, 0, w, h);

	m_projection.setToIdentity();
	m_projection.perspective(45.0f, static_cast<float>(w) / static_cast<float>(h), 0.1f, 100.0f);

	CHECK_GL_ERROR();
}

void VolumeRenderer::paintGL()
{
	auto glFunctions = QOpenGLContext::currentContext()->functions();
	glFunctions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_program.bind();
	m_program.setUniformValue("model", m_model);
	m_program.setUniformValue("view", m_view);
	m_program.setUniformValue("projection", m_projection);

	m_vao.bind();

	glFunctions->glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	m_vao.release();
	m_program.release();

	CHECK_GL_ERROR();
	update();
}

void VolumeRenderer::mousePressEvent(QMouseEvent* event)
{
	m_mousePressed = true;
	m_lastMousePos = event->pos();
}

void VolumeRenderer::mouseMoveEvent(QMouseEvent* event)
{
	if (!m_mousePressed)
		return;

	auto diff = event->pos() - m_lastMousePos;
	m_lastMousePos = event->pos();

	cameraRotation += QVector3D(diff.y(), diff.x(), 0.0f) * m_mouseSensitivity;
	cameraRotation.setX(std::max(-90.0f, std::min(90.0f, cameraRotation.x())));
	cameraRotation.setY(std::fmod(cameraRotation.y(), 360.0f));

	m_view = createViewMatrix();
}

void VolumeRenderer::mouseReleaseEvent(QMouseEvent* event)
{
	m_mousePressed = false;
}

void VolumeRenderer::wheelEvent(QWheelEvent* event)
{
	cameraPosition += QVector3D(0.0f, 0.0f, -event->angleDelta().y() * m_zoomSensitivity);

	m_view = createViewMatrix();
}

void VolumeRenderer::setupShaders()
{
	OpenGLUtils::initShaderProgram(m_program, ":/shaders/volume.vert", ":/shaders/volume.frag");

	CHECK_GL_ERROR();
}

void VolumeRenderer::setupGeometry()
{
	auto glFunctions = QOpenGLContext::currentContext()->functions();

	std::vector<QVector3D> vertices{
		{-0.5f, -0.5f, -0.5f},
		{ 0.5f, -0.5f, -0.5f},
		{ 0.5f,  0.5f, -0.5f},
		{-0.5f,  0.5f, -0.5f},
		{-0.5f, -0.5f,  0.5f},
		{ 0.5f, -0.5f,  0.5f},
		{ 0.5f,  0.5f,  0.5f},
		{-0.5f,  0.5f,  0.5f}
	};

	std::vector<uint32_t> indices{
		0, 1, 2, 2, 3, 0,    // Back face
		4, 5, 6, 6, 7, 4,    // Front face
		0, 1, 5, 5, 4, 0,    // Bottom face
		2, 3, 7, 7, 6, 2,    // Top face
		0, 3, 7, 7, 4, 0,    // Left face
		1, 2, 6, 6, 5, 1     // Right face
	};

	m_vao.create();
	m_vao.bind();

	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));

	m_ibo.create();
	m_ibo.bind();
	m_ibo.allocate(indices.data(), indices.size() * sizeof(uint32_t));

	// Position
	glFunctions->glEnableVertexAttribArray(0);
	glFunctions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), nullptr);

	m_vao.release();
	m_vbo.release();
	m_ibo.release();

	CHECK_GL_ERROR();
}

QMatrix4x4 VolumeRenderer::createViewMatrix() const
{
	QMatrix4x4 view;
	view.translate(-cameraPosition);
	view.rotate(cameraRotation.x(), QVector3D(1.0f, 0.0f, 0.0f));
	view.rotate(cameraRotation.y(), QVector3D(0.0f, 1.0f, 0.0f));
	view.rotate(cameraRotation.z(), QVector3D(0.0f, 0.0f, 1.0f));
	return view;
}
