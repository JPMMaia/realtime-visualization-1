#include "Utilities.h"
#include "EngineException.h"

#include <codecvt>
#include <locale>
#include <fstream>

using namespace std;
using namespace OpenGLEngine;

bool Utilities::FileExists(const std::wstring& filename)
{
	std::fstream fileStream(filename);

	return fileStream.good();
}

std::wstring Utilities::GetFilename(const std::wstring& filename)
{
	std::wstring searchString(L"/");

	auto begin = std::find_end(filename.begin(), filename.end(), searchString.begin(), searchString.end());
	if (begin == filename.end())
		begin = filename.begin();
	else
		++begin;

	std::wstring dot(L".");
	auto end = std::find_end(begin, filename.end(), dot.begin(), dot.end());

	return std::wstring(begin, end);
}
std::wstring Utilities::GetFileExtension(const std::wstring& filename)
{
	std::wstring dot(L".");
	auto end = std::find_end(filename.begin(), filename.end(), dot.begin(), dot.end());
	if (end == filename.end())
		ThrowEngineException(L"Filename has no extension.");

	return std::wstring(end + 1, filename.end());
}
std::wstring Utilities::GetFilePath(const std::wstring& filename)
{
	std::wstring searchString(L"/");

	auto location = std::find_end(filename.begin(), filename.end(), searchString.begin(), searchString.end());
	if (location == filename.end())
		return L"";

	return std::wstring(filename.begin(), location + 1);
}

wstring Utilities::StringToWString(const string& value)
{
	using convertType = codecvt_utf8<wchar_t>;
	wstring_convert<convertType, wchar_t> converter;

	return converter.from_bytes(value);
}
string Utilities::WStringToString(const wstring& value)
{
	using convertType = codecvt_utf8<wchar_t>;
	wstring_convert<convertType, wchar_t> converter;

	return converter.to_bytes(value);
}

void Utilities::ReadData(const std::wstring& filename, std::vector<char>& buffer)
{
	using namespace std;

	// Open file for reading in binary mode, and seek to the end of file immediately:
	ifstream file(filename, ios::in | ios::binary | ios::ate);
	if (!file.good())
		throw runtime_error("Couldn't open file " + Utilities::WStringToString(filename));

	// Get size of file and seek to the begin of file:
	auto size = file.tellg();
	file.seekg(0, ios::beg);

	// Read content of file:
	buffer.resize(static_cast<uint32_t>(size));
	file.read(buffer.data(), size);
	if (!file.good())
		throw runtime_error("Error while reading file " + Utilities::WStringToString(filename));
}
void Utilities::WriteData(const std::wstring& filename, const std::vector<char>& buffer)
{
	using namespace std;

	// Open file for writing in binary mode:
	ofstream file(filename, ios::out | ios::binary);
	if (!file.good())
		throw runtime_error("Couldn't open file " + Utilities::WStringToString(filename));

	// Write contents to file:
	file.write(buffer.data(), buffer.size());
	if (!file.good())
		throw runtime_error("Error while writing file " + Utilities::WStringToString(filename));
}
