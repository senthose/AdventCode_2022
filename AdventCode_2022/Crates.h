#pragma once

#include "AdventParser.h"
#include <vector>
#include <stack>

namespace Crates {
	const int MoveType1 = 9000;
	const int MoveType2 = 9001;

	typedef std::stack<char> CrateStack;

	class CrateCrane {
	public:
		CrateCrane();

		void AddCreateToStack(size_t inIndex, const char inCrateLetter);
		void MoveCrates(int inNumCrates, size_t inFromIndex, size_t inToIndex);
		void MoveCrates2(int inNumCrates, size_t inFromIndex, size_t inToIndex);

		const std::vector<CrateStack>& GetCrateStacks() const { return CrateStacks; };

		void Reset();
	private:
		std::vector<CrateStack> CrateStacks;
	};

	class CratesParser : public AdventParser::ParserInterface {
	public:
		CratesParser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

		std::string GetTopLine() const;

		void SetMoveType(int inMoveType) {CurrentMoveType = inMoveType;}
	private:
		void ParseCraneLines();
		void ParseCraneLine(const std::string& inCraneLine);
		void ParseMoveLine(const std::string& inMoveLine);

		enum class ParsingMode {
			Crane,
			Moves,
		};

		std::vector<std::string> CraneLines;
		CrateCrane Crane;
		ParsingMode CurrentParsingMode;
		int CurrentMoveType;
	};
}