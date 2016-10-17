#include "MoleculesBuffer.h"
#include "EngineException.h"

using namespace RTV;
using namespace RTV::BufferTypes;

void MoleculesBuffer::Create(QOpenGLShaderProgram* program, const BufferTypes::MoleculesProgramAttributeLocations& attributeLocations)
{
	// Create buffer:
	if (!m_buffer.create())
		ThrowEngineException(L"Failed to create buffer.");

	if (!m_buffer.bind())
		ThrowEngineException(L"Failed to bind buffer.");

	m_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	ThrowIfGLError();

	// Create vertex array object:
	{
		if (!m_vertices.create())
			ThrowEngineException(L"Failed to create buffer.");
		
		m_vertices.bind();
		ThrowIfGLError();

		program->enableAttributeArray(attributeLocations.PositionW);
		program->enableAttributeArray(attributeLocations.Color);
		program->setAttributeBuffer(attributeLocations.PositionW, GL_FLOAT, VertexType::PositionWOffset(), VertexType::PositionWTupleSize, VertexType::Stride());
		program->setAttributeBuffer(attributeLocations.Color, GL_FLOAT, VertexType::ColorOffset(), VertexType::ColorTupleSize, VertexType::Stride());

		m_vertices.release();
	}

	m_buffer.release();
}

void MoleculesBuffer::Destroy()
{
	m_vertices.destroy();
	m_buffer.destroy();
}

void MoleculesBuffer::Allocate(const void* data, size_t sizeInBytes)
{
	if(!m_buffer.bind())
		ThrowEngineException(L"Failed to bind buffer.");

	m_buffer.allocate(data, sizeInBytes);
	ThrowIfGLError();
}
void MoleculesBuffer::Allocate(size_t sizeInBytes)
{
	m_buffer.allocate(static_cast<int>(sizeInBytes));
	ThrowIfGLError();
}

void MoleculesBuffer::Bind()
{
	if (!m_buffer.bind())
		ThrowEngineException(L"Failed to bind buffer.");

	m_vertices.bind();
}
void MoleculesBuffer::Release()
{
	m_vertices.release();
	m_buffer.release();
}
