#include "Graphics.h"
#include "EngineException.h"
#include "ShaderProgramCompiler.h"

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

void Graphics::OnResize(int, int)
{
}

void Graphics::Update()
{
}
void Graphics::Render()
{
	// Clear color and depth buffers:
	m_openGL.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render molecules layer:
	{
		auto moleculesShaderProgram = m_programs.at("MoleculesShaderProgram").get();

		// Bind shader pipeline for use:
		if (!moleculesShaderProgram->bind())
			ThrowEngineException(L"Program failed to bind.");

		// Set uniforms:
		{
			const auto& viewMatrix = m_camera->GetViewMatrix();
			auto inverseViewMatrix = viewMatrix.inverted();
			const auto& projectionMatrix = m_camera->GetProjectionMatrix();
			float viewMatrixDeterminantCubicRoot = std::cbrt(viewMatrix.determinant());
			auto viewProjectionMatrix = projectionMatrix * viewMatrix;
			auto inverseViewProjectionMatrix = viewMatrix.inverted() * projectionMatrix.inverted();

			moleculesShaderProgram->setUniformValue("u_aspectRatio", m_camera->GetAspectRatio());
			moleculesShaderProgram->setUniformValue("u_viewMatrix", viewMatrix);
			moleculesShaderProgram->setUniformValue("u_inverseViewMatrix", inverseViewMatrix);
			moleculesShaderProgram->setUniformValue("u_projectionMatrix", projectionMatrix);
			moleculesShaderProgram->setUniformValue("u_viewProjectionMatrix", viewProjectionMatrix);
			moleculesShaderProgram->setUniformValue("u_inverseViewProjectionMatrix", inverseViewProjectionMatrix);
			moleculesShaderProgram->setUniformValue("u_eyePositionW", m_camera->GetPosition());
			moleculesShaderProgram->setUniformValue("u_viewMatrixDeterminantCubicRoot", viewMatrixDeterminantCubicRoot);
			moleculesShaderProgram->setUniformValue("u_lights[0].Strength", QVector3D(0.8f, 0.8f, 0.8f));
			moleculesShaderProgram->setUniformValue("u_lights[0].FalloffStart", 100.0f);
			moleculesShaderProgram->setUniformValue("u_lights[0].FalloffEnd", 200.0f);
			moleculesShaderProgram->setUniformValue("u_lights[0].Position", QVector3D(0.0f, 0.0f, -100.0f));
			moleculesShaderProgram->setUniformValue("u_ambientIntensity", QVector3D(0.25f, 0.25f, 0.25f));
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
void Graphics::ClearAllRenderItems()
{
	for (auto& renderLayer : m_renderItemLayers)
		renderLayer.clear();

	m_allRenderItems.clear();
}

OpenGL& Graphics::GetOpenGL()
{
	return m_openGL;
}

void Graphics::InitializeShaders()
{
	// Molecules shader:
	{
		ShaderProgramCompiler compiler;
		compiler.AddVertexShader(s_shadersFolder, "Molecules.Vertex");
		compiler.AddGeometryShader(s_shadersFolder, "Molecules.Geometry");
		compiler.AddFragmentShader(s_shadersFolder, "Molecules.Fragment");
		m_programs.emplace("MoleculesShaderProgram", compiler.Compile());
	}
}

void Graphics::DrawRenderItems(const std::vector<IRenderItem*>& renderItems, QOpenGLShaderProgram* program)
{
	for (const auto& renderItem : renderItems)
	{
		renderItem->Render(&m_openGL, program);
	}
}

