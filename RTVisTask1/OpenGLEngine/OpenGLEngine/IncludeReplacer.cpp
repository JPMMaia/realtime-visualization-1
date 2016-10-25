#include "IncludeReplacer.h"

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
