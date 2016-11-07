#pragma once

#include "OpenGLEngine/IScene.h"
#include "OpenGLEngine/Graphics.h"
#include "OpenGLEngine/MeshGeometry.h"
#include "Commons.h"

namespace RTVis
{
	class MoleculesScene : public OpenGLEngine::IScene
	{
	public:
		void Initialize(OpenGLEngine::Graphics* graphics, const std::vector<Atom>& atoms);

	private:
		void InitializeGeometry(const std::vector<Atom>& atoms);
		void InitializeRenderItems(OpenGLEngine::Graphics* graphics);

	private:
		std::unordered_map<std::string, OpenGLEngine::MeshGeometry> m_meshes;
	};
}
