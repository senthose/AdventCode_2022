#pragma once
#include <string>
#include <vector>
#include <functional>
#include <array>

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
	void RemoveNonDigits(std::string& inString);

	typedef std::function<void(void)> DayFunction;

	typedef std::array<DayFunction, 25> AdventArray;

	template<class ParserT>
	DayFunction MakeFunction(const std::string& inFilePath)
	{
		return [inFilePath]()
		{
			ParserT parser;
			Parse(inFilePath.data(), parser);
		};
	}
}