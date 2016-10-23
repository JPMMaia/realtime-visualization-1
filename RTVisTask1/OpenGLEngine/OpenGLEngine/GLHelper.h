#pragma once

#include <QtGui/QOpenGLShaderProgram>

namespace OpenGLEngine
{
	namespace GLHelper
	{
		GLint GetUniformLocation(const QOpenGLShaderProgram& program, const std::string& name);
		GLint GetAttributeLocation(const QOpenGLShaderProgram& program, const std::string& name);
	}
}
