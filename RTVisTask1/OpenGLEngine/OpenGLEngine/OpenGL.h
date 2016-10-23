#pragma once

#include <QtOpenGL/qglfunctions.h>

namespace OpenGLEngine
{
	class OpenGL : public QOpenGLFunctions
	{
	public:
		OpenGL() = default;

		void Initialize();
	};
}