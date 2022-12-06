// AdventCode_2022.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "AdventParser.h"
#include "CalorieCount.h"
#include "RPS.h"
#include "Rucksack.h"
#include "Crates.h"
#include "SignalDevice.h"

int main()
{
    //Day 1
    std::cout << "Parsing Elf Calories" << std::endl;

    CalorieCount::CalorieCountParser CalorieParser;

    AdventParser::Parse("InputFiles\\ElfCalories.txt", CalorieParser);

    std::cout << "Most calories " << CalorieParser.GetMostCalories() << std::endl;

    int First, Second, Third;
    CalorieParser.GetTopThreeCalories(First, Second, Third);
    std::cout << "Top three calories " << First << " " << Second << " " << Third << " Total " << First + Second + Third << std::endl;

    //Day 2
    std::cout << "Hosting Rock Paper Scissors Tournament" << std::endl;

    ElfRPS::RPSParser RPS;

    AdventParser::Parse("InputFiles\\RPSTournament.txt", RPS);

    std::cout << "Tournament outcome. Score: " << RPS.GetScore() << std::endl;

    RPS.SetMode(ElfRPS::Mode2);

    AdventParser::Parse("InputFiles\\RPSTournament.txt", RPS);

    std::cout << "Tournament outcome with correct directions. Score: " << RPS.GetScore() << std::endl;

    //Day 3
    std::cout << "Getting the rucksack priorities" << std::endl;

    Rucksack::RucksackParser Rucksacks;

    AdventParser::Parse("InputFiles\\Rucksacks.txt", Rucksacks);

    std::cout << "Sum of priorities " << Rucksacks.GetSumOfPriorities() << std::endl;

    std::cout << "Sum of badge priorities " << Rucksacks.GetSumOfBadgePriorities() << std::endl;

    //Day 5
    std::cout << "Reorganizing crates for unloaded from the ship" << std::endl;

    Crates::CratesParser Crates;

    AdventParser::Parse("InputFiles\\Crates.txt", Crates);

    std::cout << "The top line of reorganized crates " << Crates.GetTopLine() << std::endl;

    Crates.SetMoveType(Crates::MoveType2);
    AdventParser::Parse("InputFiles\\Crates.txt", Crates);

    std::cout << "After noticing the crane is model 9001 the new order is " << Crates.GetTopLine() << std::endl;
    
    //Day 6
    std::cout << "Attempting to find the start of the signal device..." << std::endl;

    SignalDevice::SignalDeviceParser SignalDevice;

    AdventParser::Parse("InputFiles\\DeviceSignal.txt", SignalDevice);

    std::cout << "Found the start of the buffer at index " << SignalDevice.GetStartOfBuffer() << std::endl;

    std::cout << "Found the /real/ start of the buffer at index " << SignalDevice.GetStartOfRealBuffer() << std::endl;
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
