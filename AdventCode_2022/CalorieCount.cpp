#include "CalorieCount.h"

#include <algorithm>

using namespace CalorieCount;
using namespace AdventParser;

CalorieCountParser::CalorieCountParser()
	:Elves()
	,LastLine()
{
}

void CalorieCountParser::OnStartParse()
{
	Elves.clear();
	LastLine.clear();
}

void CalorieCountParser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	if (LastLine.empty())
	{
		Elves.push_back(Elf());
	}

	Elf& currentElf = Elves.back();

	if (!inLine.empty()) {
		int calories = std::stoi(inLine);
		currentElf.TotalCalories += calories;
	}

	LastLine = inLine;
}

void CalorieCountParser::OnEndParse()
{
	auto ElfSort = [](const Elf& A, const Elf& B) -> bool
	{
		return A.TotalCalories > B.TotalCalories;
	};

	std::sort(Elves.begin(), Elves.end(), ElfSort);
}

int CalorieCountParser::GetMostCalories() const
{
	int mostCalories = 0;

	if (Elves.size() > 0)
	{
		mostCalories = Elves[0].TotalCalories;
	}

	return mostCalories;
}

void CalorieCountParser::GetTopThreeCalories(int& outFirst, int& outSecond, int& outThird) const
{
	outFirst = 0;
	outSecond = 0;
	outThird = 0;

	if (Elves.size() > 0) {
		outFirst = Elves[0].TotalCalories;
	}

	if (Elves.size() > 1) {
		outSecond = Elves[1].TotalCalories;
	}

	if (Elves.size() > 2) {
		outThird = Elves[2].TotalCalories;
	}
}