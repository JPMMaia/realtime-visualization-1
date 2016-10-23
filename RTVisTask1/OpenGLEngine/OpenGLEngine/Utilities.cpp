#include "Utilities.h"

#include <codecvt>
#include <locale>

using namespace std;
using namespace OpenGLEngine;

wstring Utilities::StringToWString(const string& value)
{
	using convertType = codecvt_utf8<wchar_t>;
	wstring_convert<convertType, wchar_t> converter;

	return converter.from_bytes(value);
}
string Utilities::WStringtoString(const wstring& value)
{
	using convertType = codecvt_utf8<wchar_t>;
	wstring_convert<convertType, wchar_t> converter;

	return converter.to_bytes(value);
}
