#include "sliceviewerwidget.h"

#include <vector>
#include <QVector3D>


// Macro to check for OpenGL errors
#define CHECK_GL_ERROR() checkOpenGLError(__FILE__, __LINE__)


SliceViewerWidget::SliceViewerWidget(QWidget* parent)
	: QOpenGLWidget(parent)
{
}

SliceViewerWidget::~SliceViewerWidget()
{
	makeCurrent();

	m_vbo.destroy();
	m_vao.destroy();

	doneCurrent();
}

void SliceViewerWidget::initializeGL()
{
	m_glFunctions = QOpenGLContext::currentContext()->functions();
	m_glFunctions->initializeOpenGLFunctions();
	m_glFunctions->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	setupShaders();
	setupGeometry();
}

void SliceViewerWidget::resizeGL(int w, int h)
{
	m_glFunctions->glViewport(0, 0, w, h);

	m_projectionMatrix.setToIdentity();
	m_projectionMatrix.perspective(45.0f, w / float(h), 0.1f, 100.0f);
}

void SliceViewerWidget::paintGL()
{
	m_glFunctions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_program.bind();
	m_program.setUniformValue(0, m_projectionMatrix);

	m_vao.bind();

	m_glFunctions->glDrawArrays(GL_TRIANGLES, 0, 3);

	m_vao.release();
	m_program.release();

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
	std::vector<QVector3D> vertices{
		{ -0.5, -0.5, -1.0f },
		{  0.5, -0.5, -1.0f },
		{  0.0,  0.5, -1.0f },
	};

	m_vao.create();
	m_vao.bind();

	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));

	m_glFunctions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), nullptr);
	m_glFunctions->glEnableVertexAttribArray(0);

	m_vbo.release();
	m_vao.release();

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

		qDebug() << "OpenGL Error:" << error << "in" << file << "at line" << line;
	}
}
