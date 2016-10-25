#include "ShaderProgramCompiler.h"
#include "EngineException.h"
#include "IncludeReplacer.h"
#include "../../src/glsw.h"

#include <memory>

#include <QtGui/QOpenGLShader>

using namespace OpenGLEngine;

void ShaderProgramCompiler::AddVertexShader(const std::wstring& folderPath, const std::string& shaderName)
{
	m_vertexShader = std::make_unique<QOpenGLShader>(QOpenGLShader::Vertex);
	{
		auto shaderSourceCode = std::string(glswGetShader(shaderName.c_str()));
		IncludeReplacer::ReplaceIncludes(folderPath, shaderSourceCode);

		if (!m_vertexShader->compileSourceCode(shaderSourceCode.c_str()))
			ThrowEngineException(L"Vertex shader failed to compile: " + m_vertexShader->log().toStdWString());
	}
}
void ShaderProgramCompiler::AddGeometryShader(const std::wstring& folderPath, const std::string& shaderName)
{
	m_geometryShader = std::make_unique<QOpenGLShader>(QOpenGLShader::Geometry);
	{
		auto shaderSourceCode = std::string(glswGetShader(shaderName.c_str()));
		IncludeReplacer::ReplaceIncludes(folderPath, shaderSourceCode);

		if (!m_geometryShader->compileSourceCode(shaderSourceCode.c_str()))
			ThrowEngineException(L"Geometry shader failed to compile: " + m_geometryShader->log().toStdWString());
	}
}
void ShaderProgramCompiler::AddFragmentShader(const std::wstring& folderPath, const std::string& shaderName)
{
	m_fragmentShader = std::make_unique<QOpenGLShader>(QOpenGLShader::Fragment);
	{
		auto shaderSourceCode = std::string(glswGetShader(shaderName.c_str()));
		IncludeReplacer::ReplaceIncludes(folderPath, shaderSourceCode);

		if (!m_fragmentShader->compileSourceCode(shaderSourceCode.c_str()))
			ThrowEngineException(L"Fragment shader failed to compile: " + m_fragmentShader->log().toStdWString());
	}
}

std::unique_ptr<QOpenGLShaderProgram> ShaderProgramCompiler::Compile() const
{
	auto program = std::make_unique<QOpenGLShaderProgram>();

	if (!program->addShader(m_vertexShader.get()))
		ThrowEngineException(L"Failed to add vertex shader to shader program.");

	if (!program->addShader(m_geometryShader.get()))
		ThrowEngineException(L"Failed to add geometry shader to shader program.");

	if (!program->addShader(m_fragmentShader.get()))
		ThrowEngineException(L"Failed to add fragment shader to shader program.");

	if (!program->link())
		ThrowEngineException(L"Failed to link shader program.");

	return std::move(program);
}
