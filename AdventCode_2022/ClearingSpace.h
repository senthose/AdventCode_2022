#pragma once

#include "AdventParser.h"
#include <vector>

namespace Clearing {
	struct ElfPair {
		ElfPair(int inElf1Start, int inElf1End, int inElf2Start, int inElf2End);

		int Elf1Start;
		int Elf1End;
		int Elf2Start;
		int Elf2End;

		bool IsElfCovered() const;
		bool IsElfPartialCovered() const;
	};

	class ClearingSpaceParser : public AdventParser::ParserInterface {
	public:
		ClearingSpaceParser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

		int GetNumOverlaps() const;
		int GetNumPartialOverlaps() const;
	private:
		std::vector<ElfPair> ElfPairs;
	};
}