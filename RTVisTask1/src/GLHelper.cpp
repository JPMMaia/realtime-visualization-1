#include "GLHelper.h"
#include "EngineException.h"

using namespace RTV;

GLint GLHelper::GetUniformLocation(const QOpenGLShaderProgram& program, const std::string& name)
{
	auto location = program.uniformLocation(name.c_str());
	if (location == -1)
		ThrowEngineException(L"Failed to get uniform location of '" + Utilities::StringToWString(name) + L"'");

	return location;
}

GLint GLHelper::GetAttributeLocation(const QOpenGLShaderProgram& program, const std::string& name)
{
	auto location = program.attributeLocation(name.c_str());
	if (location == -1)
		ThrowEngineException(L"Failed to get attribute location of '" + Utilities::StringToWString(name) + L"'");

	return location;
}
