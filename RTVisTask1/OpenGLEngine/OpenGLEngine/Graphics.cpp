#include "Graphics.h"
#include "EngineException.h"
#include "DefaultScene.h"

using namespace OpenGLEngine;


Graphics::Graphics(ICamera* camera) :
	m_camera(camera)
{
}

void Graphics::Initialize()
{
	m_openGL.Initialize();
	InitializeShaders();
	
	m_scene = std::make_unique<DefaultScene>();
	m_scene->Initialize(this);
}

void Graphics::OnResize(int width, int height)
{
	// Calculate aspect ratio:
	auto aspectRatio = qreal(width) / qreal(height ? height : 1);

	// Set near plane to 3.0, far plane to 7.0, field of view 45 degrees:
	const auto zNear = 3.0;
	const auto zFar = 7.0;
	const auto fov = 45.0;

	// Reset projection:
	m_projectionMatrix.setToIdentity();

	// Set perspective projection:
	m_projectionMatrix.perspective(fov, aspectRatio, zNear, zFar);
}

void Graphics::Update()
{
	/*// Decrease angular speed (friction)
	m_angularSpeed *= 0.99;

	// Stop rotation when speed goes below threshold
	if (m_angularSpeed < 0.01)
	{
		m_angularSpeed = 0.0;
	}
	else
	{
		// Update rotation
		m_rotation = QQuaternion::fromAxisAndAngle(m_rotationAxis, m_angularSpeed) * m_rotation;
	}*/
}
void Graphics::Render()
{
	// Clear color and depth buffers:
	m_openGL.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind shader pipeline for use:
	if (!m_program.bind())
		ThrowEngineException(L"Program failed to bind.");

	// Set model-view-projection matrix:
	{
		// Calculate model view transformation:
		QMatrix4x4 matrix;
		matrix.translate(0.0, 0.0, -5.0);
		matrix.rotate(m_rotation);

		auto viewMatrix = QMatrix4x4();
		viewMatrix.setToIdentity();
		//auto viewMatrix = m_camera->GetViewMatrix();
		auto projectionMatrix = m_camera->GetProjectionMatrix();

		m_program.setUniformValue("mvp_matrix", m_projectionMatrix * viewMatrix * matrix);
	}

	DrawRenderItems();

	// Unbind shader pipeline:
	m_program.release();
}

void Graphics::AddRenderItem(RenderItem&& renderItem)
{
	m_allRenderItems.push_back(std::move(renderItem));
}

OpenGL& Graphics::GetOpenGL()
{
	return m_openGL;
}

void Graphics::InitializeShaders()
{
	// Compile vertex shader
	if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
		ThrowEngineException(L"Vertex shader failed to compile.");

	// Compile fragment shader
	if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
		ThrowEngineException(L"Fragment shader failed to compile.");

	// Link shader pipeline
	if (!m_program.link())
		ThrowEngineException(L"Program failed to link.");
}

void Graphics::DrawRenderItems()
{
	for(const auto& renderItem : m_allRenderItems)
	{
		renderItem.Render(&m_openGL, &m_program);
	}
}
