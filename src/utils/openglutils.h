#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QDebug>
#include <QOpenGLShaderProgram>

#define CHECK_GL_ERROR() OpenGLUtils::checkError(__FUNCSIG__, __LINE__)

class OpenGLUtils
{
public:
	static QOpenGLFunctions* initializeOpenGLFunctions()
	{
		auto glFunctions = QOpenGLContext::currentContext()->functions();
		Q_ASSERT_X(glFunctions, "OpenGLUtils::initializeOpenGLFunctions", "No valid OpenGL context found");
		glFunctions->initializeOpenGLFunctions();
		return glFunctions;
	}

	static void initShaderProgram(QOpenGLShaderProgram& program, const QString& vertexShader, const QString& fragmentShader)
	{
		if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShader))
			qFatal() << "Failed to compile vertex shader:" << program.log();

		if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShader))
			qFatal() << "Failed to compile fragment shader:" << program.log();

		if (!program.link())
			qFatal() << "Failed to link shader program:" << program.log();
	}

	static void checkError(const char* file, int line)
	{
		auto glFunctions = QOpenGLContext::currentContext()->functions();
		GLenum err;
		while ((err = glFunctions->glGetError()) != GL_NO_ERROR)
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
};