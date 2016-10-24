#pragma once

#include "OpenGL.h"

class QOpenGLShaderProgram;

namespace OpenGLEngine
{
	struct IRenderItem
	{
	public:
		virtual ~IRenderItem() = default;

		virtual void Render(OpenGL* openGL, QOpenGLShaderProgram* program) const = 0;
	};
}
