#pragma once

#include <QtGui/QVector2D>
#include <QtGui/QVector3D>

namespace OpenGLEngine
{
	namespace VertexTypes
	{
		struct PositionTextureVertexType
		{
			QVector3D Position;
			QVector2D TextureCoordinates;
		};

		struct PositionColorRadiusVertexType
		{
			QVector3D Position;
			QVector3D Color;
			float Radius;
		};

		using DefaultVertexType = PositionTextureVertexType;
	}
}
