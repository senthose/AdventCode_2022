#pragma once
#include <string>
#include <vector>

namespace AdventParser {
	class ParserInterface {
	public:
		virtual void OnStartParse() = 0;
		virtual void ParseLine(const int inLineNumber, const std::string& inLine) = 0;
		virtual void OnEndParse() = 0;

		virtual ~ParserInterface() {}
	};

	void Parse(const char* inFileName, ParserInterface& inParser);

	typedef std::vector<std::string> VectorString;

	VectorString SplitString(const std::string& inString, const char inDelimiter);

	void RemoveCharFromString(std::string& inString, const char inCharToRemove);
}