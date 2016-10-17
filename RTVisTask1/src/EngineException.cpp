#include "EngineException.h"

#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace RTV;
using namespace std;

EngineException::EngineException(const std::wstring& message, const std::wstring& functionName, const std::wstring& filename, int lineNumber) :
	m_message(message),
	m_functionName(functionName),
	m_filename(filename),
	m_lineNumber(lineNumber)
{
#ifdef _WIN32
	OutputDebugStringW(m_message.c_str());
#endif
}

wstring EngineException::ToString() const
{
	wstringstream ss;

	ss << L"Message: " << m_message << endl;
	ss << L"Function Name: " << m_functionName << endl;
	ss << L"Filename: " << m_filename << endl;
	ss << L"Line Number: " << m_lineNumber << endl;

	return ss.str();
}
