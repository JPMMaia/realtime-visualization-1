#pragma once

#include <memory>
#include <QOpenGLShaderProgram>
#include <unordered_map>

namespace OpenGLEngine
{
	class ShaderProgramCompiler
	{
	public:
		void AddDefine(const std::string& define, const std::string& value);
		void AddVertexShader(const std::wstring& folderPath, const std::string& shaderName);
		void AddGeometryShader(const std::wstring& folderPath, const std::string& shaderName);
		void AddFragmentShader(const std::wstring& folderPath, const std::string& shaderName);

		std::unique_ptr<QOpenGLShaderProgram> Compile() const;

	private:
		void ReadSourceCode(const std::wstring& folderPath, const std::string& shaderName, std::string& sourceCode) const;

	private:
		std::unique_ptr<QOpenGLShader> m_vertexShader;
		std::unique_ptr<QOpenGLShader> m_geometryShader;
		std::unique_ptr<QOpenGLShader> m_fragmentShader;
		std::unordered_map<std::string, std::string> m_defines;
	};
}
