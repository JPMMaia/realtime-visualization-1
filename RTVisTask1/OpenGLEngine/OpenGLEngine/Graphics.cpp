#include "Graphics.h"
#include "EngineException.h"
#include "DefaultScene.h"
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

		// Set uniforms:
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

