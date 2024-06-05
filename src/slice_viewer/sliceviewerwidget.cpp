#include "sliceviewerwidget.h"

#include <QMouseEvent>
#include <QVector3D>

#include <vector>

// Macro to check for OpenGL errors
#define CHECK_GL_ERROR() checkOpenGLError(__FUNCSIG__, __LINE__)


SliceViewerWidget::SliceViewerWidget(QWidget* parent)
	: QOpenGLWidget(parent)
{
}

SliceViewerWidget::~SliceViewerWidget()
{
	makeCurrent();

	m_vbo.destroy();
	m_vao.destroy();
	m_texture.destroy();

	doneCurrent();
}

void SliceViewerWidget::load(const VolumeData& volume)
{
	// Convert the data to floats and normalize to [0, 1]
	std::vector<float> data(volume.data.size());
	for (size_t i = 0; i < volume.data.size(); i++)
		data[i] = volume.data[i] / 4095.0f; 

	// Set Dimensions
	int width = 1;
	int height = 1;
	int depth = 1;
	if (volume.dimensions.size() >= 1)
		width = volume.dimensions[0];
	if (volume.dimensions.size() >= 2)
		height = volume.dimensions[1];
	if (volume.dimensions.size() >= 3)
		depth = volume.dimensions[2];

	if (m_texture.isCreated())
		m_texture.destroy();

	m_texture.create();
	m_texture.setWrapMode(QOpenGLTexture::ClampToEdge);
	m_texture.setMinificationFilter(QOpenGLTexture::Linear);
	m_texture.setMagnificationFilter(QOpenGLTexture::Linear);

	m_texture.setFormat(QOpenGLTexture::R32F);
	m_texture.setSize(width, height, depth);
	m_texture.allocateStorage();
	m_texture.setData(QOpenGLTexture::Red, QOpenGLTexture::Float32, data.data());

	CHECK_GL_ERROR();

	update();
}

void SliceViewerWidget::setWindowLevel(float level)
{
	m_windowLevel = level;
	update();
}

void SliceViewerWidget::setWindowWidth(float width)
{
	m_windowWidth = width;
	update();
}

void SliceViewerWidget::setWindowing(bool enabled)
{
	m_windowing = enabled;
	update();
}

void SliceViewerWidget::initializeGL()
{
	m_glFunctions = QOpenGLContext::currentContext()->functions();
	m_glFunctions->initializeOpenGLFunctions();
	m_glFunctions->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	setupShaders();
	setupGeometry();

	setupTexture();
}

void SliceViewerWidget::resizeGL(int w, int h)
{
	m_glFunctions->glViewport(0, 0, w, h);
}

void SliceViewerWidget::paintGL()
{
	m_glFunctions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_glFunctions->glActiveTexture(GL_TEXTURE0);

	m_texture.bind();

	m_exitPointShader.bind();
	m_exitPointShader.setUniformValue("slice", m_sliceLevel);
	m_exitPointShader.setUniformValue("windowLevel", m_windowLevel);
	m_exitPointShader.setUniformValue("windowWidth", m_windowWidth);
	m_exitPointShader.setUniformValue("enableWindowing", m_windowing);

	m_vao.bind();

	m_glFunctions->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	m_vao.release();
	m_exitPointShader.release();
	m_texture.release();

	CHECK_GL_ERROR();
}

void SliceViewerWidget::wheelEvent(QWheelEvent* event)
{
	auto delta = event->angleDelta().y();
	if (delta > 0)
	{
		m_sliceLevel = std::min(1.0f, m_sliceLevel + 0.01f);
	}
	else
	{
		m_sliceLevel = std::max(0.0f, m_sliceLevel - 0.01f);
	}

	emit sliceLevelChanged(m_sliceLevel * m_texture.depth());
	update();
}

void SliceViewerWidget::resizeEvent(QResizeEvent* event)
{
	int size = qMin(event->size().width(), event->size().height());
	resize(size, size);
}

void SliceViewerWidget::setupShaders()
{
	QOpenGLShader vertexShader(QOpenGLShader::Vertex);
	QOpenGLShader fragmentShader(QOpenGLShader::Fragment);

	vertexShader.compileSourceFile(":/shaders/slice_viewer.vert");
	fragmentShader.compileSourceFile(":/shaders/slice_viewer.frag");

	m_exitPointShader.addShader(&vertexShader);
	m_exitPointShader.addShader(&fragmentShader);
	m_exitPointShader.link();

	CHECK_GL_ERROR();
}

void SliceViewerWidget::setupGeometry()
{
	std::vector<Vertex> vertices{
		{{ -1.0, -1.0, -1.0f }, { 0.0f, 0.0f }},
		{{  1.0, -1.0, -1.0f }, { 1.0f, 0.0f }},
		{{ -1.0,  1.0, -1.0f }, { 0.0f, 1.0f }},
		{{  1.0,  1.0, -1.0f }, { 1.0f, 1.0f }},
	};

	m_vao.create();
	m_vao.bind();

	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

	// Position
	m_glFunctions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
	m_glFunctions->glEnableVertexAttribArray(0);

	// Texture coordinates
	m_glFunctions->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoord)));
	m_glFunctions->glEnableVertexAttribArray(1);

	m_vbo.release();
	m_vao.release();

	CHECK_GL_ERROR();
}

void SliceViewerWidget::setupTexture()
{
	// Create empty texture to avoid GL errors at startup
	m_texture.create();
	m_texture.setWrapMode(QOpenGLTexture::ClampToEdge);
	m_texture.setMinificationFilter(QOpenGLTexture::Linear);
	m_texture.setMagnificationFilter(QOpenGLTexture::Linear);

	CHECK_GL_ERROR();
}

void SliceViewerWidget::checkOpenGLError(const char* file, int line)
{
	GLenum err;
	while ((err = m_glFunctions->glGetError()) != GL_NO_ERROR)
	{
		QString error;
		switch (err) {
		case GL_INVALID_ENUM:
			error = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = "GL_INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			error = "GL_STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			error = "GL_STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			error = "GL_OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		default:
			error = QString("Unknown Error (0x%1)").arg(err, 0, 16);
			break;
		}

		qWarning() << "OpenGL Error:" << error << "in" << file << "at line" << line;
	}
}
