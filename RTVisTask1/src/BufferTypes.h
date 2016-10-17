#pragma once

#include <qmatrix4x4.h>
#include <qopenglwidget.h>

namespace RTV
{
	namespace BufferTypes
	{
		struct PassConstants
		{
			QMatrix4x4 ViewProjectionMatrix;
		};

		struct VertexType
		{
			VertexType() = default;
			VertexType(const QVector3D& positionW, const QVector3D& color) : 
				PositionW(positionW),
				Color(color)
			{
			}

			QVector3D PositionW;
			QVector3D Color;

			static constexpr int PositionWTupleSize = 3;
			static constexpr int ColorTupleSize = 3;

			static constexpr int PositionWOffset()
			{
				return offsetof(VertexType, VertexType::PositionW);
			}
			static constexpr int ColorOffset()
			{
				return offsetof(VertexType, VertexType::Color);
			}
			static constexpr int Stride()
			{
				return sizeof(VertexType);
			}
		};

		struct MoleculesProgramUniformLocations
		{
			GLint ViewProjectionMatrix;
		};

		struct MoleculesProgramAttributeLocations
		{
			GLint PositionW;
			GLint Color;
		};
	}
}
