#pragma once

#include <QVector3D>

namespace OpenGLEngine
{
	namespace ShaderBufferTypes
	{
		struct LightConstants
		{
			QVector3D Strength;		// Light color
			float FalloffStart;		// Point/Spot light only
			QVector3D Direction;	// Directional/Spot light only
			float FalloffEnd;		// Point/Spot light only
			QVector3D Position;		// Point/Spot light only
			float SpotPower;		// Spot light only
		};

		struct MaterialConstants
		{
			QVector3D FresnelR0;
			float Shininess;
		};
	}
}
