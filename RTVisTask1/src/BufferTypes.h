#pragma once

#include <qmatrix4x4.h>
#include <qopenglwidget.h>

namespace RTV
{
	struct PassConstants
	{
		QMatrix4x4 ViewProjectionMatrix;
	};

	struct VertexType
	{
		QVector3D PositionW;
		QVector3D Color;
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
