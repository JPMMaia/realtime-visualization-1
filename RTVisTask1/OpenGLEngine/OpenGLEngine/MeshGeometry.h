#pragma once

#include "GeometryGenerator.h"

#include <QtGui/QOpenGLBuffer>

namespace OpenGLEngine
{
	class MeshGeometry
	{
	public:
		explicit MeshGeometry(const GeometryGenerator::MeshData& meshData);
		~MeshGeometry();

		void BindBuffers();
		
	private:
		QOpenGLBuffer m_vertexBuffer;
		QOpenGLBuffer m_indexBuffer;
	};
}