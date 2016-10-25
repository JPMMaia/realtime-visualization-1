#pragma once

#include <future>
#include <vector>

namespace OpenGLEngine
{
	namespace Utilities
	{
		bool FileExists(const std::wstring& filename);
		std::wstring GetFilename(const std::wstring& filename);
		std::wstring GetFileExtension(const std::wstring& filename);
		std::wstring GetFilePath(const std::wstring& filename);

		std::wstring StringToWString(const std::string& value);
		std::string WStringToString(const std::wstring& value);

		void ReadData(const std::wstring& filename, std::vector<char>& buffer);
		void WriteData(const std::wstring& filename, const std::vector<char>& buffer);

		template<typename FunctionType, typename... ArgumentsType>
		std::future<typename std::result_of<FunctionType(ArgumentsType...)>::type> RunAsync(FunctionType&& function, ArgumentsType&&... arguments)
		{
			return std::async(std::launch::async, std::forward<FunctionType>(function), std::forward<ArgumentsType>(arguments)...);
		}
	}
}
