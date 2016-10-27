#pragma once

#include "IRenderItem.h"
#include "MeshGeometry.h"
#include "OpenGL.h"

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLTexture>

namespace OpenGLEngine
{
	struct DefaultRenderItem : public IRenderItem
	{
	public:
		void Render(OpenGL* openGL, QOpenGLShaderProgram* program) const override;

		const std::string& GetName() const override;

	public:
		std::string Name;
		MeshGeometry* Mesh;
		QOpenGLTexture* Texture;
		GLint PrimitiveType;
	};
}
