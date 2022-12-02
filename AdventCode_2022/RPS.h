#pragma once
#include "AdventParser.h"

namespace ElfRPS {
	const int Mode1 = 1;
	const int Mode2 = 2;

	class RPSParser : public AdventParser::ParserInterface {
	public:
		RPSParser(int inMode = Mode1);

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

		void SetMode(int inMode);

		int GetScore() const { return CurrentScore; };
	private:
		int Mode;
		int CurrentScore;
	};
}