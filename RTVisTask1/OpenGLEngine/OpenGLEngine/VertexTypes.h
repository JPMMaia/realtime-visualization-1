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

		using DefaultVertexType = PositionTextureVertexType;
	}
}
