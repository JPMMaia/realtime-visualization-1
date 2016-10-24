#pragma once

#include <QMatrix4x4>

namespace OpenGLEngine
{
	class ICamera
	{
	public:
		virtual ~ICamera() = default;

		virtual const QVector3D& GetPosition() const = 0;

		virtual const QMatrix4x4& GetViewMatrix() const = 0;
		virtual const QMatrix4x4& GetProjectionMatrix() const = 0;
	};
}