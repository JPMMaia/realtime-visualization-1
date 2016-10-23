#pragma once

#include "MeshGeometry.h"
#include "OpenGL.h"

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLTexture>

namespace OpenGLEngine
{
	class RenderItem
	{
	public:
		RenderItem(MeshGeometry* mesh, QOpenGLTexture* texture);

		void Render(OpenGL* openGL, QOpenGLShaderProgram* program) const;

	private:
		MeshGeometry* m_mesh;
		QOpenGLTexture* m_texture;
	};
}
