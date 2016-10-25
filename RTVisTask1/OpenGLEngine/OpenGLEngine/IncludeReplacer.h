#pragma once

#include "Utilities.h"

#include <iterator>
#include <vector>

namespace OpenGLEngine
{
	class IncludeReplacer
	{
	public:
		static void ReplaceIncludes(const std::wstring& inputFilename, const std::wstring& outputFilename);

		template<typename ContainerType>
		static void ReplaceIncludes(const std::wstring& folderPath, ContainerType& content);

	private:
		template<typename ContainerType>
		static bool FindAndEraseIncludeTag(ContainerType& content, std::wstring& includedFile, typename ContainerType::iterator& location);
	};

	template <typename ContainerType>
	void IncludeReplacer::ReplaceIncludes(const std::wstring& folderPath, ContainerType& content)
	{
		using namespace std;

		wstring includedFile;
		typename ContainerType::iterator location;
		while (FindAndEraseIncludeTag(content, includedFile, location))
		{
			vector<char> includedContent;
			Utilities::ReadData(folderPath + includedFile, includedContent);

			content.insert(location, includedContent.begin(), includedContent.end());
		}
	}

	template <typename ContainerType>
	bool IncludeReplacer::FindAndEraseIncludeTag(ContainerType& content, std::wstring& includedFile, typename ContainerType::iterator& location)
	{
		using namespace std;

		auto iterator = content.begin();

		typename ContainerType::iterator begin;
		do
		{
			iterator = std::find(iterator, content.end(), '#');
			if (iterator == content.end())
				return false;

			// Create iterator to the begin of the line:
			begin = ContainerType::iterator(iterator);

		} while (
			*(++iterator) != 'i' ||
			*(++iterator) != 'n' ||
			*(++iterator) != 'c' ||
			*(++iterator) != 'l' ||
			*(++iterator) != 'u' ||
			*(++iterator) != 'd' ||
			*(++iterator) != 'e' ||
			*(++iterator) != ' ' ||
			*(++iterator) != '"'
			);

		string includedFileTmp;
		includedFileTmp.reserve(48);
		while (*(++iterator) != '"')
			includedFileTmp.push_back(*iterator);
		includedFile = Utilities::StringToWString(includedFileTmp);

		// Skip end of line:
		do { ++iterator; } while (*iterator == ' ' || *iterator == '\r');
		do { ++iterator; } while (*iterator == '\n');

		// Erase #include "...":
		location = content.erase(begin, iterator);

		return true;
	}
}
