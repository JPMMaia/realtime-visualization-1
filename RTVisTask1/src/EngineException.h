#pragma once

#include "Utilities.h"

namespace RTV
{
	class EngineException
	{
	public:
		EngineException() = default;
		EngineException(const std::wstring& message, const std::wstring& functionName, const std::wstring& filename, int lineNumber);

		std::wstring ToString() const;

	private:
		std::wstring m_message;
		std::wstring m_functionName;
		std::wstring m_filename;
		int m_lineNumber = -1;
	};

#ifndef ThrowEngineException
#define ThrowEngineException(message)											\
{																				\
	std::wstring functionName = Utilities::StringToWString(__FUNCSIG__);		\
	std::wstring filename = Utilities::StringToWString(__FILE__);				\
	throw EngineException(message, functionName, filename, __LINE__);			\
}
#endif
}
