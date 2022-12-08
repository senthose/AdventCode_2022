#include "Crates.h"

using namespace Crates;

CrateCrane::CrateCrane()
	:CrateStacks()
{
}

void CrateCrane::AddCreateToStack(size_t inIndex, const char inCrateLetter)
{
	if (CrateStacks.size() < inIndex + 1)
	{
		while (CrateStacks.size() < inIndex + 1)
		{
			CrateStacks.emplace_back();
		}
	}

	CrateStack& stack = CrateStacks[inIndex];

	stack.push(inCrateLetter);
}

void CrateCrane::MoveCrates(int inNumCrates, size_t inFromIndex, size_t inToIndex)
{
	CrateStack& fromStack = CrateStacks[inFromIndex];
	CrateStack& toStack = CrateStacks[inToIndex];

	for (int i = 0; i < inNumCrates; ++i)
	{
		const char crateLetter = fromStack.top();
		fromStack.pop();
		toStack.push(crateLetter);
	}
}

void CrateCrane::MoveCrates2(int inNumCrates, size_t inFromIndex, size_t inToIndex)
{
	CrateStack& fromStack = CrateStacks[inFromIndex];
	CrateStack& toStack = CrateStacks[inToIndex];

	CrateStack swapSpace;
	for (int i = 0; i < inNumCrates; ++i)
	{
		const char crateLetter = fromStack.top();
		fromStack.pop();
		swapSpace.push(crateLetter);
	}

	for (int i = 0; i < inNumCrates; ++i)
	{
		const char crateLetter = swapSpace.top();
		swapSpace.pop();
		toStack.push(crateLetter);
	}
}

void CrateCrane::Reset()
{
	CrateStacks.clear();
}

CratesParser::CratesParser()
	:CraneLines()
	,Crane()
	,CurrentParsingMode(ParsingMode::Crane)
{
}

void CratesParser::OnStartParse()
{
	Crane.Reset();
	CurrentParsingMode = CratesParser::ParsingMode::Crane;
}

void CratesParser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	if (!inLine.empty())
	{
		switch (CurrentParsingMode)
		{
		case ParsingMode::Crane:
			CraneLines.push_back(inLine);
			break;
		case ParsingMode::Moves:
			ParseMoveLine(inLine);
			break;
		default:
			break;
		}
	}
	else
	{
		ParseCraneLines();
		CurrentParsingMode = ParsingMode::Moves;
	}
}

void CratesParser::OnEndParse()
{
}

std::string CratesParser::GetTopLine() const
{
	std::string topLine;
	const std::vector<CrateStack>& crateStacks = Crane.GetCrateStacks();

	for (const CrateStack& stack : crateStacks)
	{
		topLine.push_back(stack.top());
	}

	return topLine;
}

void CratesParser::ParseCraneLines()
{
	CraneLines.pop_back();

	for (auto it = CraneLines.rbegin(); it != CraneLines.rend(); ++it)
	{
		ParseCraneLine(*it);
	}
}

void CratesParser::ParseCraneLine(const std::string& inCraneLine)
{
	size_t lineLength = inCraneLine.size();

	size_t numCrates = (lineLength + 1)/4;

	for (size_t i = 0; i < numCrates; ++i)
	{
		const char crateLetter = inCraneLine[(i * 4) + 1];

		if (crateLetter >= 'A' && crateLetter <= 'Z')
		{
			Crane.AddCreateToStack(i, crateLetter);
		}
	}
}

void CratesParser::ParseMoveLine(const std::string& inMoveLine)
{
	AdventParser::VectorString splitMoveLine = AdventParser::SplitString(inMoveLine, ' ');

	int numCrates = std::stoi(splitMoveLine[1]);
	size_t fromIndex = static_cast<size_t>(std::stoi(splitMoveLine[3])) - 1;
	size_t toIndex = static_cast<size_t>(std::stoi(splitMoveLine[5])) - 1;

	if (CurrentMoveType == MoveType1)
	{
		Crane.MoveCrates(numCrates, fromIndex, toIndex);
	}
	else if (CurrentMoveType == MoveType2)
	{ 
		Crane.MoveCrates2(numCrates, fromIndex, toIndex);
	}
}