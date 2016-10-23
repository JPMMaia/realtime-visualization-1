#include "DefaultScene.h"
#include "Graphics.h"

using namespace OpenGLEngine;

void DefaultScene::Initialize(Graphics* graphics)
{
	InitializeGeometry();
	InitializeTextures();
	InitializeRenderItems(graphics);
}

void DefaultScene::InitializeGeometry()
{
	// Cube mesh:
	{
		auto cubeMeshData = GeometryGenerator::CreateCube();
		m_meshes.emplace(std::piecewise_construct, std::forward_as_tuple("Cube"), std::forward_as_tuple(cubeMeshData));
	}
}
void DefaultScene::InitializeTextures()
{
	// Cube texture:
	{
		auto texture = std::make_unique<QOpenGLTexture>(QImage(":/cube.jpg").mirrored());
		texture->setMinificationFilter(QOpenGLTexture::Nearest);
		texture->setMagnificationFilter(QOpenGLTexture::Linear);
		texture->setWrapMode(QOpenGLTexture::Repeat);
		m_textures.emplace("CubeTexture", std::move(texture));
	}
}
void DefaultScene::InitializeRenderItems(Graphics* graphics)
{
	// Cube render item:
	graphics->AddRenderItem(RenderItem(&m_meshes.at("Cube"), m_textures.at("CubeTexture").get()));
}
