#pragma once

#include "GeometryGenerator.h"

#include <QtGui/QOpenGLBuffer>

namespace OpenGLEngine
{
	class MeshGeometry
	{
	public:
		template<typename VertexType>
		explicit MeshGeometry(const GeometryGenerator::MeshData<VertexType>& meshData);
		~MeshGeometry();

		void BindBuffers();

		const QOpenGLBuffer& GetIndexBuffer() const;
		size_t GetIndexCount() const;
		
	private:
		QOpenGLBuffer m_vertexBuffer;
		QOpenGLBuffer m_indexBuffer;
		size_t m_indexCount;
	};

	template <typename VertexType>
	MeshGeometry::MeshGeometry(const GeometryGenerator::MeshData<VertexType>& meshData) :
		m_vertexBuffer(QOpenGLBuffer::Type::VertexBuffer),
		m_indexBuffer(QOpenGLBuffer::Type::IndexBuffer),
		m_indexCount(meshData.Indices.size())
	{
		// Create vertex buffer and transfer vertex data to it:
		m_vertexBuffer.create();
		m_vertexBuffer.bind();
		m_vertexBuffer.allocate(meshData.Vertices.data(), static_cast<int>(meshData.Vertices.size() * sizeof(VertexType)));

		// Create index buffer and transfer index data to it:
		m_indexBuffer.create();
		m_indexBuffer.bind();
		m_indexBuffer.allocate(meshData.Indices.data(), static_cast<int>(meshData.Indices.size() * sizeof(uint32_t)));
	}
}
