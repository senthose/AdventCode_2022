#pragma once
#include "AdventParser.h"
#include <vector>

namespace CalorieCount {
	struct Elf {
		int TotalCalories = 0;
	};

	class CalorieCountParser : public AdventParser::ParserInterface {
	public:
		CalorieCountParser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

		int GetMostCalories() const;

		void GetTopThreeCalories(int& outFirst, int& outSecond, int& outThird) const;

	private:
		std::vector<Elf> Elves;
		std::string LastLine;
	};
}