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