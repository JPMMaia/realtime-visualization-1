#include "EngineException.h"

#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <gl/GLU.h>

using namespace RTV;
using namespace std;

EngineException::EngineException(int error, const std::wstring& functionName, const std::wstring& filename, int lineNumber) :
	m_message(L"GLError: "),
	m_functionName(functionName),
	m_filename(filename),
	m_lineNumber(lineNumber)
{
	auto gl = QOpenGLContext::currentContext()->functions();

	do
	{
		m_message += Utilities::StringToWString(std::string(reinterpret_cast<const char*>(gluErrorString(error))));
	} 
	while ((error = gl->glGetError()) != GL_NO_ERROR);

#ifdef _WIN32
	OutputDebugStringW(ToString().c_str());
#endif
}

EngineException::EngineException(const std::wstring& message, const std::wstring& functionName, const std::wstring& filename, int lineNumber) :
	m_message(message),
	m_functionName(functionName),
	m_filename(filename),
	m_lineNumber(lineNumber)
{
	auto gl = QOpenGLContext::currentContext()->functions();

	int error;
	while ((error = gl->glGetError()) != GL_NO_ERROR)
	{
		m_message += L" glError: " + Utilities::StringToWString(std::string(reinterpret_cast<const char*>(gluErrorString(error)))) + L"\n";
	}

#ifdef _WIN32
	OutputDebugStringW(ToString().c_str());
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
