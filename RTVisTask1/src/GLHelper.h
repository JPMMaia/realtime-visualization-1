#pragma once

#include <QOpenGLShaderProgram>

namespace RTV
{
	namespace GLHelper
	{
		GLint GetUniformLocation(const QOpenGLShaderProgram& program, const std::string& name);
		GLint GetAttributeLocation(const QOpenGLShaderProgram& program, const std::string& name);
	}
}
