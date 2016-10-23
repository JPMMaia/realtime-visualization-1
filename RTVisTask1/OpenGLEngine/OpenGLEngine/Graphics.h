#pragma once

#include "OpenGL.h"
#include "DefaultScene.h"
#include "RenderItem.h"

#include <array> 
#include <memory>

#include <QtGui/QOpenGLShaderProgram>

namespace OpenGLEngine
{
	class Graphics
	{
	public:
		Graphics() = default;

		void Initialize();
		
		void OnResize(int width, int height);

		void Update();
		void Render();

		void AddRenderItem(RenderItem&& renderItem);

		OpenGL& GetOpenGL();

	private:
		void InitializeShaders();
		void DrawRenderItems();

	private:
		OpenGL m_openGL;

		QOpenGLShaderProgram m_program;
		QMatrix4x4 m_projectionMatrix;

		QVector3D m_rotationAxis;
		qreal m_angularSpeed;
		QQuaternion m_rotation;

		std::unique_ptr<DefaultScene> m_scene;
		std::vector<RenderItem> m_allRenderItems;
	};
}
