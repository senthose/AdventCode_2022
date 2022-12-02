#include "RPS.h"
#include <array>

using namespace ElfRPS;

enum class Shape {
	Rock = 0,
	Paper = 1,
	Scissor = 2,
	Invalid,
};

enum class Result {
	Win,
	Lose,
	Draw,
	Invalid,
};


const std::array<Result, 9> RPSMatrix = { Result::Draw, Result::Win, Result::Lose, //Rock
								   Result::Lose, Result::Draw, Result::Win, //Paper
								   Result::Win, Result::Lose, Result::Draw};//Scissors

Shape GetShape(const char inChar)
{
	switch (inChar)
	{
	case 'A': return Shape::Rock;
	case 'B': return Shape::Paper;
	case 'C': return Shape::Scissor;
	case 'X': return Shape::Rock;
	case 'Y': return Shape::Paper;
	case 'Z': return Shape::Scissor;

	default: return Shape::Invalid;
	}
}

Result GetResult(const char inChar)
{
	switch (inChar)
	{
	case 'X': return Result::Lose;
	case 'Y': return Result::Draw;
	case 'Z': return Result::Win;

	default: return Result::Invalid;
	}
}

int GetScore(Result inOutcome, Shape inPlayerPlay)
{
	const int playerValue = static_cast<int>(inPlayerPlay) + 1;

	switch (inOutcome)
	{
	case Result::Win:
		return 6 + playerValue;
	case Result::Lose:
		return 0 + playerValue;
	case Result::Draw:
		return 3 + playerValue;
	default:
		return 0;
	}
}

int CalculateScore(const Shape inElfPlay, const Shape inPlayerPlay)
{
	if (inElfPlay == Shape::Invalid || inPlayerPlay == Shape::Invalid)
	{
		return 0;
	}

	const int elfOffset = static_cast<int>(inElfPlay);
	const int playerOffset = static_cast<int>(inPlayerPlay);

	Result outcome = RPSMatrix[(3 * elfOffset) + playerOffset];

	return GetScore(outcome, inPlayerPlay);
}

int CalculateScoreFromResult(const Shape inElfPlay, const Result inExpectedOutcome)
{
	if (inElfPlay == Shape::Invalid || inExpectedOutcome == Result::Invalid)
	{
		return 0;
	}

	const int elfOffset = static_cast<int>(inElfPlay);

	//I would use a span, but compiler I am using is too old.
	const Result* Outcomes = &RPSMatrix[3 * elfOffset];

	int playerOffset = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (Outcomes[i] == inExpectedOutcome) {
			playerOffset = i;
		}
	}

	const Shape playerShape = static_cast<Shape>(playerOffset);
	return GetScore(inExpectedOutcome, playerShape);
}

RPSParser::RPSParser(int inMode)
	:Mode(inMode)
	,CurrentScore(0)
{
}

void RPSParser::OnStartParse()
{
	CurrentScore = 0;
}

void RPSParser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	if (!inLine.empty())
	{
		const Shape ElfPlay = GetShape(inLine[0]);
		if (Mode == Mode1) {
			const Shape PlayerRespone = GetShape(inLine[2]);

			CurrentScore += CalculateScore(ElfPlay, PlayerRespone);
		}
		else if (Mode == Mode2) {
			const Result ResultNeeded = GetResult(inLine[2]);

			CurrentScore += CalculateScoreFromResult(ElfPlay, ResultNeeded);
		}
	}
}

void RPSParser::OnEndParse()
{
}

void RPSParser::SetMode(int inMode)
{
	Mode = inMode;
}