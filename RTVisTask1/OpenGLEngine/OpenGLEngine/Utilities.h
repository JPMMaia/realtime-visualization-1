#pragma once

#include <string>

namespace OpenGLEngine
{
	namespace Utilities
	{
		std::wstring StringToWString(const std::string& value);
		std::string WStringtoString(const std::wstring& value);
	}
}
