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
			VertexType(const QVector3D& positionW, const QVector3D& color, float radius) : 
				PositionW(positionW),
				Color(color),
				Radius(radius)
			{
			}

			QVector3D PositionW;
			QVector3D Color;
			float Radius;
		};
	}
}
