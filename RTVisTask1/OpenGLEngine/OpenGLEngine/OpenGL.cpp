#include "OpenGL.h"

using namespace OpenGLEngine;

void OpenGL::Initialize()
{
	// Initialize openGL functions:
	QOpenGLFunctions::initializeOpenGLFunctions();

	// Set clear color:
	glClearColor(0, 0, 0, 1);

	// Enable depth buffer:
	glEnable(GL_DEPTH_TEST);

	// Enable back face culling:
	glEnable(GL_CULL_FACE);
}
