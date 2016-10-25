#pragma once

#include <QVector3D>

namespace OpenGLEngine
{
	namespace ShaderBufferTypes
	{
		struct Light
		{
			QVector3D Strength;		// Light color
			float FalloffStart;		// Point/Spot light only
			QVector3D Direction;	// Directional/Spot light only
			float FalloffEnd;		// Point/Spot light only
			QVector3D Position;		// Point/Spot light only
			float SpotPower;		// Spot light only
		};
	}
}
