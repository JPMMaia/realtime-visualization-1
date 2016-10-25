﻿#include "IncludeReplacer.h"
#include "Utilities.h"

using namespace OpenGLEngine;
using namespace std;

void IncludeReplacer::ReplaceIncludes(const std::wstring& inputFilename, const std::wstring& outputFilename)
{
	auto filePath = Utilities::GetFilePath(inputFilename);

	vector<char> content;
	Utilities::ReadData(inputFilename, content);

	vector<char>::iterator location;
	wstring includedFile;
	while (FindAndEraseIncludeTag(content, includedFile, location))
	{
		vector<char> includedContent;
		Utilities::ReadData(filePath + includedFile, includedContent);

		content.insert(location, includedContent.begin(), includedContent.end());
	}

	Utilities::WriteData(outputFilename, content);
}

bool IncludeReplacer::FindAndEraseIncludeTag(std::vector<char>& content, std::wstring& includedFile, std::vector<char>::iterator& location)
{
	auto iterator = content.begin();

	std::vector<char>::iterator begin;
	do
	{
		iterator = std::find(iterator, content.end(), '#');
		if (iterator == content.end())
			return false;

		// Create iterator to the begin of the line:
		begin = std::vector<char>::iterator(iterator);

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
