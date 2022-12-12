#include "AdventParser.h"

#include <fstream>

using namespace AdventParser;

void AdventParser::Parse(const char* inFileName, ParserInterface& inParser)
{
	std::ifstream ParseFile(inFileName);
	std::string ParseString;
	int LineNumber = 1;

	inParser.OnStartParse();

	if (ParseFile.is_open())
	{
		while (!ParseFile.eof())
		{
			std::getline(ParseFile, ParseString);
			inParser.ParseLine(LineNumber, ParseString);
			++LineNumber;
		}
	}

	inParser.OnEndParse();
}

VectorString AdventParser::SplitString(const std::string& inString, const char inDelimiter)
{
	VectorString splitStrings;
	const size_t stringLength = inString.size();
	size_t lastIndex = 0;
	for (size_t i = 0; i < stringLength; ++i)
	{
		if (inString[i] == inDelimiter)
		{
			std::string subString = inString.substr(lastIndex, i - lastIndex);
			if (!subString.empty())
			{
				splitStrings.push_back(inString.substr(lastIndex, i - lastIndex));
			}
			lastIndex = i + 1;
		}
	}
	splitStrings.push_back(inString.substr(lastIndex));

	return splitStrings;
}

void AdventParser::RemoveCharFromString(std::string& inString, const char inCharToRemove)
{
	size_t charIndex = inString.find(inCharToRemove);
	while (charIndex != std::string::npos)
	{
		inString.erase(charIndex);
		charIndex = inString.find(':');
	}
}