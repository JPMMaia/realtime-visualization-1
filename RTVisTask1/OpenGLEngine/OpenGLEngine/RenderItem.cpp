#include "RenderItem.h"

using namespace OpenGLEngine;

RenderItem::RenderItem(MeshGeometry* mesh, QOpenGLTexture* texture) :
	m_mesh(mesh),
	m_texture(texture)
{
}

void RenderItem::Render(OpenGL* openGL, QOpenGLShaderProgram* program) const
{
	// Use texture unit 0 which contains cube.png:
	m_texture->bind();
	program->setUniformValue("texture", 0);

	// Bind mesh buffers:
	m_mesh->BindBuffers();

	// Offset for position:
	quintptr offset = 0;

	// Tell OpenGL pipeline how to locate vertex position data:
	auto vertexLocation = program->attributeLocation("a_position");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexTypes::DefaultVertexType));

	// Offset for texture coordinate:
	offset += sizeof(QVector3D);

	// Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
	auto textureCoordinatesLocation = program->attributeLocation("a_texcoord");
	program->enableAttributeArray(textureCoordinatesLocation);
	program->setAttributeBuffer(textureCoordinatesLocation, GL_FLOAT, offset, 2, sizeof(VertexTypes::DefaultVertexType));

	// Draw cube geometry using indices from VBO 1:
	openGL->glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, nullptr);
}
