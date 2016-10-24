#include "Graphics.h"
#include "EngineException.h"
#include "DefaultScene.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/type_ptr.inl>
#include "../../src/glsw.h"

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

	// Render texture layer:
	{
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

		DrawRenderItems(m_renderItemLayers[static_cast<size_t>(RenderLayer::Texture)], textureShaderProgram);

		// Unbind shader pipeline:
		textureShaderProgram->release();
	}

	// Render molecules layer:
	{
		auto moleculesShaderProgram = m_programs.at("MoleculesShaderProgram").get();

		// Bind shader pipeline for use:
		if (!moleculesShaderProgram->bind())
			ThrowEngineException(L"Program failed to bind.");

		// Set view-projection matrix:
		{
			const auto& viewMatrix = m_camera->GetViewMatrix();
			const auto& projectionMatrix = m_camera->GetProjectionMatrix();

			moleculesShaderProgram->setUniformValue("u_viewProjectionMatrix", projectionMatrix * viewMatrix);
		}

		DrawRenderItems(m_renderItemLayers[static_cast<size_t>(RenderLayer::Molecules)], moleculesShaderProgram);

		// Unbind shader pipeline:
		moleculesShaderProgram->release();
	}
}

void Graphics::AddRenderItem(std::unique_ptr<IRenderItem>&& renderItem, std::initializer_list<RenderLayer> renderLayers)
{
	for (auto renderLayer : renderLayers)
	{
		m_renderItemLayers[static_cast<SIZE_T>(renderLayer)].push_back(renderItem.get());
	}

	m_allRenderItems.push_back(std::move(renderItem));
}

OpenGL& Graphics::GetOpenGL()
{
	return m_openGL;
}

void Graphics::InitializeShaders()
{
	// Texture program shader:
	{
		auto program = std::make_unique<QOpenGLShaderProgram>();
		
		if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
			ThrowEngineException(L"Vertex shader failed to compile.");

		if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
			ThrowEngineException(L"Fragment shader failed to compile.");

		if (!program->link())
			ThrowEngineException(L"Program failed to link.");

		m_programs.emplace("TextureShaderProgram", std::move(program));
	}

	// Molecules shader:
	{
		auto vertexShader = std::make_unique<QOpenGLShader>(QOpenGLShader::Vertex);
		{
			auto vertexShaderSourceCode = glswGetShader("Molecules.Vertex");
			if (!vertexShader->compileSourceCode(vertexShaderSourceCode))
				ThrowEngineException(L"Vertex shader failed to compile.");
		}

		auto fragmentShader = std::make_unique<QOpenGLShader>(QOpenGLShader::Fragment);
		{
			auto fragmentShaderSourceCode = glswGetShader("Molecules.Fragment");
			if (!fragmentShader->compileSourceCode(fragmentShaderSourceCode))
				ThrowEngineException(L"Fragment shader failed to compile.");
		}

		{
			auto program = std::make_unique<QOpenGLShaderProgram>();

			if (!program->addShader(vertexShader.get()))
				ThrowEngineException(L"Failed to add vertex shader to molecules shader program.");

			if (!program->addShader(fragmentShader.get()))
				ThrowEngineException(L"Failed to add fragment shader to molecules shader program.");

			if (!program->link())
				ThrowEngineException(L"Failed to link molecules shader program.");

			m_programs.emplace("MoleculesShaderProgram", std::move(program));
		}
	}
}

void Graphics::DrawRenderItems(const std::vector<IRenderItem*>& renderItems, QOpenGLShaderProgram* program)
{
	for (const auto& renderItem : renderItems)
	{
		renderItem->Render(&m_openGL, program);
	}
}

