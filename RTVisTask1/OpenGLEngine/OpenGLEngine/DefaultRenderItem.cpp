#include "DefaultRenderItem.h"

using namespace OpenGLEngine;

void DefaultRenderItem::Render(OpenGL* openGL, QOpenGLShaderProgram* program) const
{
	// Bind texture:
	this->Texture->bind();
	program->setUniformValue("texture", 0);

	// Bind mesh buffers:
	this->Mesh->BindBuffers();

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
	openGL->glDrawElements(this->PrimitiveType, static_cast<GLsizei>(this->Mesh->GetIndexCount()), GL_UNSIGNED_INT, nullptr);
}
