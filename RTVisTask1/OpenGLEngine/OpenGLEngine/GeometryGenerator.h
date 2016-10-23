#pragma once

#include "VertexTypes.h"

#include <vector>

namespace OpenGLEngine
{
	class GeometryGenerator
	{
	public:

		struct MeshData
		{
			std::vector<VertexTypes::DefaultVertexType> Vertices;
			std::vector<uint32_t> Indices;
		};

	public:
		static MeshData CreateCube();
	};
}
