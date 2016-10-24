#pragma once

#include "OpenGL.h"
#include "DefaultScene.h"
#include "IRenderItem.h"
#include "ICamera.h"

#include <QtGui/QOpenGLShaderProgram>
#include <unordered_map>

namespace OpenGLEngine
{
	class Graphics
	{
	public:
		Graphics() = default;
		explicit Graphics(ICamera* camera);

		void Initialize();
		
		void OnResize(int width, int height);

		void Update();
		void Render();

		void AddRenderItem(std::unique_ptr<IRenderItem>&& renderItem);

		OpenGL& GetOpenGL();

	private:
		void InitializeShaders();
		void DrawRenderItems(QOpenGLShaderProgram* program);

	private:
		OpenGL m_openGL;

		std::vector<std::unique_ptr<IRenderItem>> m_allRenderItems;
		ICamera* m_camera = nullptr;

		std::unordered_map<std::string, std::unique_ptr<QOpenGLShaderProgram>> m_programs;

		//QOpenGLShaderProgram m_program;
	};
}
