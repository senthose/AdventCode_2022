#include "Rucksack.h"
#include <bitset>
#include <cmath>

using namespace Rucksack;

typedef std::bitset<52> RucksackSet;

RucksackSet GetItemSet(const std::string& inItems)
{
	RucksackSet set;
	for (const char c : inItems)
	{
		size_t position = 0;

		if (c >= 'a' && c <= 'z')
		{
			position = static_cast<size_t>(c) - 'a';
		}
		if (c >= 'A' && c <= 'Z')
		{
			position = (static_cast<size_t>(c) - 'A') + 26;
		}

		set.set(position);
	}

	return set;
}

RucksackContents::RucksackContents(const std::string& inItems)
	:Items(inItems)
	,SharedItem(0)
	,Priority(0)
{
	std::string::size_type halfLength = Items.size() / 2;

	const std::string leftCompartment = Items.substr(0, halfLength);
	const std::string rightCompartment = Items.substr(halfLength, halfLength);

	RucksackSet leftSet = GetItemSet(leftCompartment);
	RucksackSet rightSet = GetItemSet(rightCompartment);

	RucksackSet prioritySet = leftSet & rightSet;

	size_t priorityValue = static_cast<size_t>(std::log2(prioritySet.to_ullong()));

	if (priorityValue > 25)
	{
		SharedItem = 'A' + (priorityValue - 26);
	}
	else
	{
		SharedItem = 'a' + priorityValue;
	}

	Priority = priorityValue + 1;
}

RucksackGroup::RucksackGroup(const TripleRucksack& inRucksacks)
{
	RucksackSet set1 = GetItemSet(std::get<0>(inRucksacks));
	RucksackSet set2 = GetItemSet(std::get<1>(inRucksacks));
	RucksackSet set3 = GetItemSet(std::get<2>(inRucksacks));

	RucksackSet prioritySet = set1 & set2 & set3;

	size_t priorityValue = static_cast<size_t>(std::log2(prioritySet.to_ullong()));

	if (priorityValue > 25)
	{
		SharedBadge = 'A' + (priorityValue - 26);
	}
	else
	{
		SharedBadge = 'a' + priorityValue;
	}

	Priority = priorityValue + 1;
}

RucksackParser::RucksackParser()
	:Rucksacks()
{
}

void RucksackParser::OnStartParse()
{
	Rucksacks.clear();
}

void RucksackParser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	if (!inLine.empty())
	{
		Rucksacks.push_back(RucksackContents(inLine));
	}
}

void RucksackParser::OnEndParse()
{
}

int RucksackParser::GetSumOfPriorities() const
{
	int sumOfPriorities = 0;
	for (const RucksackContents& rucksack : Rucksacks)
	{
		sumOfPriorities += rucksack.Priority;
	}

	return sumOfPriorities;
}

int RucksackParser::GetSumOfBadgePriorities() const
{
	int sumOfBadgePriorities = 0;

	for (size_t i = 0; i < Rucksacks.size(); i += 3)
	{
		TripleRucksack elfGroup = std::make_tuple(Rucksacks[i].Items, Rucksacks[i + 1].Items, Rucksacks[i + 2].Items);
		RucksackGroup group(elfGroup);

		sumOfBadgePriorities += group.Priority;
	}

	return sumOfBadgePriorities;
}