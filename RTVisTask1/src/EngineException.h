#pragma once

#include "Utilities.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>

namespace RTV
{
	class EngineException
	{
	public:
		EngineException() = default;
		EngineException(int error, const std::wstring& functionName, const std::wstring& filename, int lineNumber);
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

#ifndef ThrowIfGLError
#define ThrowIfGLError()														\
{																				\
	auto error = QOpenGLContext::currentContext()->functions()->glGetError();	\
	if(error != GL_NO_ERROR)\
	{\
		std::wstring functionName = Utilities::StringToWString(__FUNCSIG__);		\
		std::wstring filename = Utilities::StringToWString(__FILE__);				\
		throw EngineException(error, functionName, filename, __LINE__);		\
	}\
}
#endif
}
