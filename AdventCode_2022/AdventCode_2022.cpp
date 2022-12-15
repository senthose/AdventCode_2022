// AdventCode_2022.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "AdventParser.h"
#include "CalorieCount.h"
#include "RPS.h"
#include "Rucksack.h"
#include "ClearingSpace.h"
#include "Crates.h"
#include "SignalDevice.h"
#include "DirectoryCrawler.h"
#include "Trees.h"
#include "Rope.h"
#include "DeviceCPU.h"
#include "Monkey.h"
#include "HillPathfinding.h"
#include "DistressSignal.h"
#include "Sand.h"
#include "Beacons.h"

void Day1()
{
	std::cout << "Parsing Elf Calories" << std::endl;

	CalorieCount::CalorieCountParser CalorieParser;

	AdventParser::Parse("InputFiles\\ElfCalories.txt", CalorieParser);

	std::cout << "Most calories " << CalorieParser.GetMostCalories() << std::endl;

	int First, Second, Third;
	CalorieParser.GetTopThreeCalories(First, Second, Third);
	std::cout << "Top three calories " << First << " " << Second << " " << Third << " Total " << First + Second + Third << std::endl;
}

void Day2()
{
	std::cout << "Hosting Rock Paper Scissors Tournament" << std::endl;

	ElfRPS::RPSParser RPS;

	AdventParser::Parse("InputFiles\\RPSTournament.txt", RPS);

	std::cout << "Tournament outcome. Score: " << RPS.GetScore() << std::endl;

	RPS.SetMode(ElfRPS::Mode2);

	AdventParser::Parse("InputFiles\\RPSTournament.txt", RPS);

	std::cout << "Tournament outcome with correct directions. Score: " << RPS.GetScore() << std::endl;
}

void Day3()
{
	std::cout << "Getting the rucksack priorities" << std::endl;

	Rucksack::RucksackParser Rucksacks;

	AdventParser::Parse("InputFiles\\Rucksacks.txt", Rucksacks);

	std::cout << "Sum of priorities " << Rucksacks.GetSumOfPriorities() << std::endl;

	std::cout << "Sum of badge priorities " << Rucksacks.GetSumOfBadgePriorities() << std::endl;
}

void Day4()
{
	std::cout << "Optimizing clearing space" << std::endl;

	Clearing::ClearingSpaceParser ClearingParser;

	AdventParser::Parse("InputFiles\\ElfCleanupPairs.txt", ClearingParser);

	std::cout << "Found " << ClearingParser.GetNumOverlaps() << " overlaps and " << ClearingParser.GetNumPartialOverlaps() << " partial overlaps" << std::endl;
}

void Day5()
{
	std::cout << "Reorganizing crates for unloaded from the ship" << std::endl;

	Crates::CratesParser Crates;

	AdventParser::Parse("InputFiles\\Crates.txt", Crates);

	std::cout << "The top line of reorganized crates " << Crates.GetTopLine() << std::endl;

	Crates.SetMoveType(Crates::MoveType2);
	AdventParser::Parse("InputFiles\\Crates.txt", Crates);

	std::cout << "After noticing the crane is model 9001 the new order is " << Crates.GetTopLine() << std::endl;
}

void Day6()
{
	std::cout << "Attempting to find the start of the signal device..." << std::endl;

	SignalDevice::SignalDeviceParser SignalDevice;

	AdventParser::Parse("InputFiles\\DeviceSignal.txt", SignalDevice);

	std::cout << "Found the start of the buffer at index " << SignalDevice.GetStartOfBuffer() << std::endl;

	std::cout << "Found the /real/ start of the buffer at index " << SignalDevice.GetStartOfRealBuffer() << std::endl;
}

void Day7()
{
	std::cout << "Crawling the communicator files..." << std::endl;

	DirectoryCrawler::DirectoryCrawlerParser DirectoryCrawler;

	AdventParser::Parse("InputFiles\\CommunicatorCommands.txt", DirectoryCrawler);

	std::cout << "Found " << DirectoryCrawler.GetTotalUnder100k() << " total bytes of directories up to 100000" << std::endl;

	std::cout << "The smallest directory to delete to make space was " << DirectoryCrawler.GetSmallestToDelete() << std::endl;
}

void Day8()
{
	std::cout << "Checking for Tree Cover" << std::endl;

	Trees::TreeParser Trees;

	AdventParser::Parse("InputFiles\\TreeHeights.txt", Trees);

	std::cout << "Found " << Trees.GetNumTreesVisible() << " visible from outside the forest" << std::endl;

	std::cout << "The highest possible \"scenic score\" found was " << Trees.GetHighestScenicScore() << std::endl;
}

void Day9()
{
	std::cout << "Distracting self by running rope simulation " << std::endl;

	Rope::RopeParser Rope;

	AdventParser::Parse("InputFiles\\RopeMoves.txt", Rope);

	std::cout << "The tail moved through " << Rope.GetNumTailSpaces(1) << " spaces " << std::endl;

	std::cout << "The end of the longer rope moved through " << Rope.GetNumTailSpaces(9) << " spaces " << std::endl;
}

void Day10()
{
	std::cout << "Attempting to decode the video signal from the device" << std::endl;

	DeviceCPU::Parser CPUParser;

	AdventParser::Parse("InputFiles\\DeviceInstructions.txt", CPUParser);

	std::cout << "Cumulative signal strength: " << CPUParser.GetSignalSum() << std::endl;

	std::cout << "Decoding the stream of instructions: " << std::endl;

	std::cout << CPUParser.GetDeviceScreen() << std::endl;
}

void Day11()
{
	std::cout << "Chasing down some monkeys" << std::endl;

	MonkeyBusiness::MonkeyParser Monkeys;
	MonkeyBusiness::MonkeyParser MonkeysNotWorried;

	AdventParser::Parse("InputFiles\\Monkeys.txt", Monkeys);

	Monkeys.DoRounds(20);

	std::cout << "After 20 rounds of monkey " << Monkeys.GetMonkeyBusinessValue() << std::endl;

	AdventParser::Parse("InputFiles\\Monkeys.txt", MonkeysNotWorried);
	MonkeysNotWorried.SetWorried(false);
	MonkeysNotWorried.DoRounds(10000);

	std::cout << "After 10000 rounds monkey business with more worry " << MonkeysNotWorried.GetMonkeyBusinessValue() << std::endl;
}

void Day12()
{
	std::cout << "Finding the shortest path to the top of that hill" << std::endl;

	Hill::HillParser Hills;

	AdventParser::Parse("InputFiles\\HillMap.txt", Hills);

	std::cout << "Shortest path length to top of hill: " << Hills.GetShortestPath() << std::endl;

	std::cout << "Shortst scenic path length: " << Hills.GetShortestScenicPath() << std::endl;
}

int main()
{
	AdventParser::AdventArray adventDays;

	adventDays[0] = Day1;
	adventDays[1] = Day2;
	adventDays[2] = Day3;
	adventDays[3] = Day4;
	adventDays[4] = Day5;
	adventDays[5] = Day6;
	adventDays[6] = Day7;
	adventDays[7] = Day8;
	adventDays[8] = Day9;
	adventDays[9] = Day10;
	adventDays[10] = Day11;
	adventDays[11] = Day12;
	adventDays[12] = AdventParser::MakeFunction<Distress::DistressSignalParser>("InputFiles\\DistressSignal.txt");
	adventDays[13] = AdventParser::MakeFunction<Sand::CaveSandParser>("InputFiles\\Cave.txt");
	adventDays[14] = AdventParser::MakeFunction<Beacon::Parser>("InputFiles\\Sensors.txt");

    std::string inputBuffer;

    std::cout << "Enter day:";

    //std::cin >> inputBuffer;
	inputBuffer = "15";

    int inputDay = 0;
    bool validInput = false;

	for (char& inputChar : inputBuffer)
	{
		inputChar =  std::tolower(inputChar);
	}

	if (inputBuffer == "all")
	{
		for (AdventParser::DayFunction day : adventDays)
		{
			if (day)
			{
				day();
			}
		}

		return 0;
	}

    try {
        inputDay = std::stoi(inputBuffer);
        validInput = true;
    }
    catch (std::exception e)
    {
        std::cout << "Invalid day. Exiting..." << std::endl;

		return 0;
    }

    if (validInput)
    {
		--inputDay;
		if (inputDay >= 0 && inputDay < adventDays.size())
		{
			AdventParser::DayFunction& day = adventDays[inputDay];
			if (day)
			{
				day();
			}
			else
			{
				std::cout << "Day not complete yet." << std::endl;
			}
		}
    }

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
