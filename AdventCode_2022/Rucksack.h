#pragma once
#include "AdventParser.h"
#include <vector>
#include <tuple>


namespace Rucksack {
	struct RucksackContents {
		RucksackContents(const std::string& inItems);

		std::string Items;
		char SharedItem;
		int Priority;
	};

	typedef std::tuple<std::string, std::string, std::string> TripleRucksack;
	struct RucksackGroup {
		RucksackGroup(const TripleRucksack& inRucksacks);

		char SharedBadge;
		int Priority;
	};

	class RucksackParser : public AdventParser::ParserInterface {
	public:
		RucksackParser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

		int GetSumOfPriorities() const;

		int GetSumOfBadgePriorities() const;
	private:
		std::vector<RucksackContents> Rucksacks;
	};
}