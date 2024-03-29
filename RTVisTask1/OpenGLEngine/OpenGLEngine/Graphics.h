#pragma once

#include "OpenGL.h"
#include "IRenderItem.h"
#include "ICamera.h"
#include "RenderLayer.h"

#include <QtGui/QOpenGLShaderProgram>
#include <unordered_map>
#include <memory>

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
		IRenderItem* GetRenderItem(const std::string& name) const;
		void ClearAllRenderItems();

		OpenGL& GetOpenGL();

		void SetLightDiffuseIntensity(float value);
		void SetLightAmbientIntensity(float value);

	private:
		void InitializeShaders();
		void DrawRenderItems(const std::vector<IRenderItem*>& renderItems, QOpenGLShaderProgram* program);

	private:
		OpenGL m_openGL;
		std::unordered_map<std::string, std::unique_ptr<QOpenGLShaderProgram>> m_programs;
		ICamera* m_camera = nullptr;

		std::vector<std::unique_ptr<IRenderItem>> m_allRenderItems;
		std::vector<IRenderItem*> m_renderItemLayers[static_cast<SIZE_T>(RenderLayer::Count)];

		std::wstring s_shadersFolder = L"src/shader/";
		float m_lightDiffuseIntensity = 0.5f;
		float m_lightAmbientIntensity = 0.05f;
	};
}
