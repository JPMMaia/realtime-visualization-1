#pragma once

#include "OpenGLEngine/IRenderItem.h"
#include "OpenGLEngine/MeshGeometry.h"
#include "OpenGLEngine/ShaderBufferTypes.h"

namespace RTVis
{
	class MoleculesRenderItem : public OpenGLEngine::IRenderItem
	{
	public:
		void Render(OpenGLEngine::OpenGL* openGL, QOpenGLShaderProgram* program) const override;

		const std::string& GetName() const override;

	public:
		std::string Name;
		OpenGLEngine::MeshGeometry* Mesh;
		OpenGLEngine::ShaderBufferTypes::MaterialConstants MaterialConstants;
		GLuint PrimitiveType;
	};
}
