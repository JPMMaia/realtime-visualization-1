#include "MoleculesRenderItem.h"

#include <QOpenGLShaderProgram>

using namespace RTVis;
using namespace OpenGLEngine;

void MoleculesRenderItem::Render(OpenGLEngine::OpenGL* openGL, QOpenGLShaderProgram* program) const
{
	// Bind mesh buffers:
	this->Mesh->BindBuffers();

	// Offset for position:
	auto stride = static_cast<int>(sizeof(VertexTypes::PositionColorRadiusVertexType));
	quintptr offset = 0;

	// Tell OpenGL pipeline how to locate vertex position data:
	auto vertexLocation = program->attributeLocation("vs_in_positionW");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, stride);	
	offset += sizeof(QVector3D);

	// Tell OpenGL programmable pipeline how to locate vertex color data:
	auto colorLocation = program->attributeLocation("vs_in_color");
	program->enableAttributeArray(colorLocation);
	program->setAttributeBuffer(colorLocation, GL_FLOAT, offset, 3, stride);
	offset += sizeof(QVector3D);

	// Tell OpenGL programmable pipeline how to locate vertex radius data:
	auto radiusLocation = program->attributeLocation("vs_in_radius");
	program->enableAttributeArray(radiusLocation);
	program->setAttributeBuffer(radiusLocation, GL_FLOAT, offset, 1, stride);

	// Set material:
	{
		auto materialFresnelR0Location = program->uniformLocation("u_materialFresnelR0");
		program->setUniformValue(materialFresnelR0Location, this->MaterialConstants.FresnelR0);

		auto materialShininessLocation = program->uniformLocation("u_materialShininess");
		program->setUniformValue(materialShininessLocation, this->MaterialConstants.Shininess);
	}

	// Draw geometry:
	openGL->glDrawElements(this->PrimitiveType, static_cast<GLsizei>(this->Mesh->GetIndexCount()), GL_UNSIGNED_INT, nullptr);
}

const std::string& MoleculesRenderItem::GetName() const
{
	return this->Name;
}
