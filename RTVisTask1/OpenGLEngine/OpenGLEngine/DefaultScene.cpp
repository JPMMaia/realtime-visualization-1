#include "DefaultScene.h"
#include "Graphics.h"
#include "DefaultRenderItem.h"

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
	{
		auto cubeRenderItem = std::make_unique<DefaultRenderItem>();
		cubeRenderItem->Mesh = &m_meshes.at("Cube");
		cubeRenderItem->Texture = m_textures.at("CubeTexture").get();
		cubeRenderItem->PrimitiveType = GL_TRIANGLE_STRIP;

		graphics->AddRenderItem(std::move(cubeRenderItem));
	}
}
