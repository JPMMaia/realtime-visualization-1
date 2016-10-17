#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include "BufferTypes.h"

namespace RTV
{
	class MoleculesBuffer
	{
	public:
		MoleculesBuffer() = default;

		void Create(QOpenGLShaderProgram* program, const BufferTypes::MoleculesProgramAttributeLocations& attributeLocations);
		void Destroy();

		void Allocate(const void* data, size_t sizeInBytes);
		void Allocate(size_t sizeInBytes);

		void Bind();
		void Release();

	private:
		QOpenGLBuffer m_buffer;
		QOpenGLVertexArrayObject m_vertices;
	};
}
