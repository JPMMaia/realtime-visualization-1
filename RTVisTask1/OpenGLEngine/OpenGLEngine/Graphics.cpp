#include "Graphics.h"
#include "EngineException.h"
#include "DefaultScene.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/type_ptr.inl>

using namespace OpenGLEngine;


Graphics::Graphics(ICamera* camera) :
	m_camera(camera)
{
}

void Graphics::Initialize()
{
	m_openGL.Initialize();
	InitializeShaders();
}

void Graphics::OnResize(int width, int height)
{
}

void Graphics::Update()
{
}
void Graphics::Render()
{
	// Clear color and depth buffers:
	m_openGL.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto textureShaderProgram = m_programs.at("TextureShaderProgram").get();

	// Bind shader pipeline for use:
	if (!textureShaderProgram->bind())
		ThrowEngineException(L"Program failed to bind.");

	// Set model-view-projection matrix:
	{
		// Calculate model view transformation:
		QMatrix4x4 matrix;
		matrix.translate(0.0, 0.0, -5.0);

		const auto& viewMatrix = m_camera->GetViewMatrix();
		const auto& projectionMatrix = m_camera->GetProjectionMatrix();

		textureShaderProgram->setUniformValue("mvp_matrix", projectionMatrix * viewMatrix * matrix);
	}

	DrawRenderItems(textureShaderProgram);

	// Unbind shader pipeline:
	textureShaderProgram->release();
}

void Graphics::AddRenderItem(std::unique_ptr<IRenderItem>&& renderItem)
{
	m_allRenderItems.push_back(std::move(renderItem));
}

OpenGL& Graphics::GetOpenGL()
{
	return m_openGL;
}

void Graphics::InitializeShaders()
{
	{
		auto program = std::make_unique<QOpenGLShaderProgram>();

		// Compile vertex shader
		if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
			ThrowEngineException(L"Vertex shader failed to compile.");

		// Compile fragment shader
		if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
			ThrowEngineException(L"Fragment shader failed to compile.");

		// Link shader pipeline
		if (!program->link())
			ThrowEngineException(L"Program failed to link.");

		m_programs.emplace("TextureShaderProgram", std::move(program));
	}
}

void Graphics::DrawRenderItems(QOpenGLShaderProgram* program)
{
	for (const auto& renderItem : m_allRenderItems)
	{
		renderItem->Render(&m_openGL, program);
	}
}

