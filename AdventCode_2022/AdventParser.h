#pragma once
#include <string>

namespace AdventParser {
	class ParserInterface {
	public:
		virtual void OnStartParse() = 0;
		virtual void ParseLine(const int inLineNumber, const std::string& inLine) = 0;
		virtual void OnEndParse() = 0;

		virtual ~ParserInterface() {}
	};

	void Parse(const char* inFileName, ParserInterface& inParser);
}