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

	m_exitPointFBO.reset();

	doneCurrent();
}

void VolumeRenderer::initializeGL()
{
	auto glFunctions = OpenGLUtils::initializeOpenGLFunctions();
	glFunctions->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glFunctions->glEnable(GL_CULL_FACE);

	m_exitPointFBO = std::make_unique<QOpenGLFramebufferObject>(width(), height());
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

	m_exitPointFBO = std::make_unique<QOpenGLFramebufferObject>(w, h);

	CHECK_GL_ERROR();
}

void VolumeRenderer::paintGL()
{
	// Render back faces to get the exit points
	backFacePass();

	// Render the volume using ray casting
	volumePass();

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
	cameraPosition.setZ(std::max(m_minZoom, std::min(m_maxZoom, cameraPosition.z())));

	m_view = createViewMatrix();
}

void VolumeRenderer::setupShaders()
{
	OpenGLUtils::initShaderProgram(m_exitPointShader, ":/shaders/exitpoints.vert", ":/shaders/exitpoints.frag");
	OpenGLUtils::initShaderProgram(m_raycastShader, ":/shaders/raycasting.vert", ":/shaders/raycasting.frag");

	CHECK_GL_ERROR();
}

void VolumeRenderer::setupGeometry()
{
	std::vector<QVector3D> vertices{
		{ -0.5f, -0.5f, -0.5f}, // Left bottom back
		{ -0.5f, -0.5f,  0.5f}, // Left bottom front
		{ -0.5f,  0.5f, -0.5f}, // Left top back
		{ -0.5f,  0.5f,  0.5f}, // Left top front
		{  0.5f, -0.5f, -0.5f}, // Right bottom back
		{  0.5f, -0.5f,  0.5f}, // Right bottom front
		{  0.5f,  0.5f, -0.5f}, // Right top back
		{  0.5f,  0.5f,  0.5f}  // Right top front
	};

	std::vector<uint32_t> indices{
		0, 1, 2, 1, 3, 2,    // Left
		0, 5, 1, 0, 4, 5,    // Bottom
		0, 2, 6, 0, 6, 4,    // Back
		2, 3, 7, 2, 7, 6,    // Top
		1, 5, 7, 1, 7, 3,    // Front
		4, 6, 5, 5, 6, 7     // Right
	};

	m_vao.create();
	m_vao.bind();

	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));

	m_ibo.create();
	m_ibo.bind();
	m_ibo.allocate(indices.data(), indices.size() * sizeof(uint32_t));

	auto glFunctions = QOpenGLContext::currentContext()->functions();
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

void VolumeRenderer::backFacePass()
{
	m_exitPointFBO->bind();

	auto glFunctions = QOpenGLContext::currentContext()->functions();
	glFunctions->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glFunctions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFunctions->glCullFace(GL_FRONT);

	m_exitPointShader.bind();
	m_exitPointShader.setUniformValue("model", m_model);
	m_exitPointShader.setUniformValue("view", m_view);
	m_exitPointShader.setUniformValue("projection", m_projection);

	m_vao.bind();

	glFunctions->glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	m_vao.release();
	m_exitPointShader.release();
	m_exitPointFBO->release();

	CHECK_GL_ERROR();
}

void VolumeRenderer::volumePass()
{
	auto glFunctions = QOpenGLContext::currentContext()->functions();
	glFunctions->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
	glFunctions->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glFunctions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFunctions->glCullFace(GL_BACK);

	glFunctions->glActiveTexture(GL_TEXTURE0);
	glFunctions->glBindTexture(GL_TEXTURE_2D, m_exitPointFBO->texture());

	m_raycastShader.bind();
	m_raycastShader.setUniformValue("model", m_model);
	m_raycastShader.setUniformValue("view", m_view);
	m_raycastShader.setUniformValue("projection", m_projection);
	m_raycastShader.setUniformValue("screenSize", size());

	m_vao.bind();

	glFunctions->glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	m_vao.release();
	m_raycastShader.release();

	CHECK_GL_ERROR();
}
