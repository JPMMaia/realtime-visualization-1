#pragma once

#include <string>

namespace RTV
{
	namespace Utilities
	{
		std::wstring StringToWString(const std::string& value);
		std::string WStringtoString(const std::wstring& value);
	}
}
