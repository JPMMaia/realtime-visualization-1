#include "MeshGeometry.h"

using namespace OpenGLEngine;

MeshGeometry::MeshGeometry(const GeometryGenerator::MeshData& meshData) :
	m_vertexBuffer(QOpenGLBuffer::Type::VertexBuffer),
	m_indexBuffer(QOpenGLBuffer::Type::IndexBuffer)
{
	// Create vertex buffer and transfer vertex data to it:
	m_vertexBuffer.create();
	m_vertexBuffer.bind();
	m_vertexBuffer.allocate(meshData.Vertices.data(), static_cast<int>(meshData.Vertices.size() * sizeof(VertexTypes::DefaultVertexType)));

	// Create index buffer and transfer index data to it:
	m_indexBuffer.create();
	m_indexBuffer.bind();
	m_indexBuffer.allocate(meshData.Indices.data(), static_cast<int>(meshData.Indices.size() * sizeof(uint32_t)));
}
MeshGeometry::~MeshGeometry()
{
	m_indexBuffer.destroy();
	m_vertexBuffer.destroy();
}

void MeshGeometry::BindBuffers()
{
	m_vertexBuffer.bind();
	m_indexBuffer.bind();
}
