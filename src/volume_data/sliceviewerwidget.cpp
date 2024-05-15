#include "sliceviewerwidget.h"

#include <vector>
#include <QVector3D>


// Macro to check for OpenGL errors
#define CHECK_GL_ERROR() checkOpenGLError(__FUNCSIG__, __LINE__)


SliceViewerWidget::SliceViewerWidget(QWidget* parent)
	: QOpenGLWidget(parent), m_texture(QOpenGLTexture::Target2D)
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
	m_program.bind();
	m_vao.bind();

	m_glFunctions->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	m_vao.release();
	m_program.release();
	m_texture.release();

	CHECK_GL_ERROR();
}

void SliceViewerWidget::setupShaders()
{
	QOpenGLShader vertexShader(QOpenGLShader::Vertex);
	QOpenGLShader fragmentShader(QOpenGLShader::Fragment);

	vertexShader.compileSourceFile(":/shaders/slice_viewer.vert");
	fragmentShader.compileSourceFile(":/shaders/slice_viewer.frag");

	m_program.addShader(&vertexShader);
	m_program.addShader(&fragmentShader);
	m_program.link();

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
	QImage image(":/assets/images/painting.png");
	if (image.isNull())
	{
		qWarning() << "Failed to load image";
		return;
	}

	image = image.convertToFormat(QImage::Format_RGBA8888);
	image = image.mirrored();

	m_texture.create();
	m_texture.setWrapMode(QOpenGLTexture::ClampToEdge);
	m_texture.setMinificationFilter(QOpenGLTexture::Linear);
	m_texture.setMagnificationFilter(QOpenGLTexture::Linear);
	
	m_texture.setFormat(QOpenGLTexture::RGBA8_UNorm);
	m_texture.setSize(image.width(), image.height());
	m_texture.allocateStorage();
	m_texture.setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, image.constBits());

	if (!m_texture.isCreated())
		qWarning() << "Failed to create texture";
	else
		qDebug() << "Texture created:" << m_texture.width() << "x" << m_texture.height();

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
