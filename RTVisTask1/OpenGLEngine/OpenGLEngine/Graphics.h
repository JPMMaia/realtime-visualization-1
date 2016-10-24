#pragma once

#include "OpenGL.h"
#include "DefaultScene.h"
#include "IRenderItem.h"
#include "ICamera.h"
#include "RenderLayer.h"

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

		void AddRenderItem(std::unique_ptr<IRenderItem>&& renderItem, std::initializer_list<RenderLayer> renderLayers);

		OpenGL& GetOpenGL();

	private:
		void InitializeShaders();
		void DrawRenderItems(const std::vector<IRenderItem*>& renderItems, QOpenGLShaderProgram* program);

	private:
		OpenGL m_openGL;
		std::unordered_map<std::string, std::unique_ptr<QOpenGLShaderProgram>> m_programs;
		ICamera* m_camera = nullptr;

		std::vector<std::unique_ptr<IRenderItem>> m_allRenderItems;
		std::vector<IRenderItem*> m_renderItemLayers[static_cast<SIZE_T>(RenderLayer::Count)];
	};
}
