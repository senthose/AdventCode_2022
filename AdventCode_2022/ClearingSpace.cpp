#include "ClearingSpace.h"

using namespace Clearing;

ElfPair::ElfPair(int inElf1Start, int inElf1End, int inElf2Start, int inElf2End)
	:Elf1Start(inElf1Start)
	,Elf1End(inElf1End)
	,Elf2Start(inElf2Start)
	,Elf2End(inElf2End)
{
}

bool ElfPair::IsElfCovered() const
{
	bool isCovered = false;
	int elf1Range = Elf1End - Elf1Start;
	int elf2Range = Elf2End - Elf2Start;

	if (elf1Range >= elf2Range)
	{
		if (Elf1Start <= Elf2Start && Elf1End >= Elf2End)
		{
			isCovered = true;
		}
	}
	else
	{
		if (Elf2Start <= Elf1Start && Elf2End >= Elf1End)
		{
			isCovered = true;
		}
	}

	return isCovered;
}

bool ElfPair::IsElfPartialCovered() const
{
	bool isPartialCovered = false;


	for (int i = Elf1Start; i <= Elf1End; ++i)
	{
		if (i >= Elf2Start && i <= Elf2End)
		{
			isPartialCovered = true;
			break;
		}
	}

	return isPartialCovered;
}

ClearingSpaceParser::ClearingSpaceParser()
	:ElfPairs()
{
}

void ClearingSpaceParser::OnStartParse()
{
	ElfPairs.clear();
}

void ClearingSpaceParser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	if (!inLine.empty())
	{
		const size_t commaIndex = inLine.find(',');
		std::string elf1Str = inLine.substr(0, commaIndex);
		std::string elf2Str = inLine.substr(commaIndex + 1);

		size_t dashIndex = elf1Str.find('-');
		int elf1Start = std::stoi(elf1Str.substr(0, dashIndex));
		int elf1End = std::stoi(elf1Str.substr(dashIndex + 1));

		dashIndex = elf2Str.find('-');
		int elf2Start = std::stoi(elf2Str.substr(0, dashIndex));
		int elf2End = std::stoi(elf2Str.substr(dashIndex + 1));

		ElfPairs.push_back(ElfPair(elf1Start, elf1End, elf2Start, elf2End));
	}
}

void ClearingSpaceParser::OnEndParse()
{
}

int ClearingSpaceParser::GetNumOverlaps() const
{
	int numOverlaps = 0;
	for (const ElfPair& pair : ElfPairs)
	{
		if (pair.IsElfCovered())
		{
			++numOverlaps;
		}
	}

	return numOverlaps;
}

int ClearingSpaceParser::GetNumPartialOverlaps() const
{
	int numPartialOverlaps = 0;
	for (const ElfPair& pair : ElfPairs)
	{
		if (pair.IsElfPartialCovered())
		{
			++numPartialOverlaps;
		}
	}

	return numPartialOverlaps;
}