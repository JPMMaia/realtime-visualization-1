#include "MeshGeometry.h"

using namespace OpenGLEngine;

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

const QOpenGLBuffer& MeshGeometry::GetIndexBuffer() const
{
	return m_indexBuffer;
}

size_t MeshGeometry::GetIndexCount() const
{
	return m_indexCount;
}
