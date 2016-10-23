#include "OpenGL.h"

using namespace OpenGLEngine;

void OpenGL::Initialize()
{
	// Initialize openGL functions:
	QOpenGLFunctions::initializeOpenGLFunctions();

	// Set clear color:
	glClearColor(0.862f, 0.929f, 0.949f, 1.0f);

	// Enable depth buffer:
	glEnable(GL_DEPTH_TEST);

	// Enable back face culling:
	glEnable(GL_CULL_FACE);
}

void OpenGL::BeginRender()
{

}
