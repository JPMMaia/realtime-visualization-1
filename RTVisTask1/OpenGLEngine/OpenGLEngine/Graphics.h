#pragma once

#include "OpenGL.h"
#include "DefaultScene.h"
#include "IRenderItem.h"
#include "ICamera.h"

#include <QtGui/QOpenGLShaderProgram>

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
		void DrawRenderItems();

	private:
		OpenGL m_openGL;

		std::vector<std::unique_ptr<IRenderItem>> m_allRenderItems;
		ICamera* m_camera = nullptr;

		QOpenGLShaderProgram m_program;
	};
}
