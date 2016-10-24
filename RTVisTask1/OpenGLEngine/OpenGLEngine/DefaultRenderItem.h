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
		void Render(OpenGL* openGL, QOpenGLShaderProgram* program) const override;

	public:
		MeshGeometry* Mesh;
		QOpenGLTexture* Texture;
		GLint PrimitiveType;
	};
}
