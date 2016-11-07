#include "ShaderProgramCompiler.h"
#include "EngineException.h"
#include "IncludeReplacer.h"
#include "../../src/glsw.h"

#include <memory>

#include <QtGui/QOpenGLShader>

using namespace OpenGLEngine;

void ShaderProgramCompiler::AddDefine(const std::string& define, const std::string& value)
{
	m_defines.emplace(define, value);
}

void ShaderProgramCompiler::AddVertexShader(const std::wstring& folderPath, const std::string& shaderName)
{
	m_vertexShader = std::make_unique<QOpenGLShader>(QOpenGLShader::Vertex);
	{
		std::string sourceCode;
		ReadSourceCode(folderPath, shaderName, sourceCode);

		if (!m_vertexShader->compileSourceCode(sourceCode.c_str()))
			ThrowEngineException(L"Vertex shader failed to compile: " + m_vertexShader->log().toStdWString());
	}
}
void ShaderProgramCompiler::AddGeometryShader(const std::wstring& folderPath, const std::string& shaderName)
{
	m_geometryShader = std::make_unique<QOpenGLShader>(QOpenGLShader::Geometry);
	{
		std::string sourceCode;
		ReadSourceCode(folderPath, shaderName, sourceCode);

		if (!m_geometryShader->compileSourceCode(sourceCode.c_str()))
			ThrowEngineException(L"Geometry shader failed to compile: " + m_geometryShader->log().toStdWString());
	}
}
void ShaderProgramCompiler::AddFragmentShader(const std::wstring& folderPath, const std::string& shaderName)
{
	m_fragmentShader = std::make_unique<QOpenGLShader>(QOpenGLShader::Fragment);
	{
		std::string sourceCode;
		ReadSourceCode(folderPath, shaderName, sourceCode);
		
		if (!m_fragmentShader->compileSourceCode(sourceCode.c_str()))
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

void ShaderProgramCompiler::ReadSourceCode(const std::wstring& folderPath, const std::string& shaderName, std::string& sourceCode) const
{
	sourceCode = std::string(glswGetShader(shaderName.c_str()));
	IncludeReplacer::ReplaceIncludes(folderPath, sourceCode);

	for(auto& pair : m_defines)
	{
		sourceCode = "#define " + pair.first + " " + pair.second + "\n" + sourceCode;
	}

#if defined(DEBUG) || defined(_DEBUG)
	Utilities::WriteData(folderPath + L"Generated_" + Utilities::StringToWString(shaderName) + L".glsl", std::vector<char>(sourceCode.begin(), sourceCode.end()));
#endif
}
