#pragma once

#include "VertexTypes.h"

#include <vector>

namespace OpenGLEngine
{
	class GeometryGenerator
	{
	public:

		template<typename VertexType = VertexTypes::DefaultVertexType>
		struct MeshData
		{
			std::vector<VertexType> Vertices;
			std::vector<uint32_t> Indices;
		};

	public:

		static MeshData<VertexTypes::DefaultVertexType> CreateCube();
	};
}
