#pragma once

#include "IScene.h"
#include "MeshGeometry.h"

#include <QtGui/QOpenGLTexture>
#include <memory>
#include <unordered_map>

namespace OpenGLEngine
{
	class Graphics;

	class DefaultScene : public IScene
	{
	public:
		void Initialize(Graphics* graphics);

	private:
		void InitializeGeometry();
		void InitializeTextures();
		void InitializeRenderItems(Graphics* graphics);

	private:
		std::unordered_map<std::string, MeshGeometry> m_meshes;
		std::unordered_map<std::string, std::unique_ptr<QOpenGLTexture>> m_textures;
	};
}
