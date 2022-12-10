#include "DeviceCPU.h"

using namespace DeviceCPU;

Instruction GetInstructionFromString(const std::string& inInstructionString)
{
	if (inInstructionString == "noop")
	{
		return Instruction::Noop;
	}

	if (inInstructionString == "addx")
	{
		return Instruction::Addx;
	}

	return Instruction::Invalid;
}

CPU::CPU()
	:DeviceScreen()
	,SpriteBuffer()
	,SignalPoints()
	,CycleCounter(0)
	,RegisterX(1)
{
	UpdateSpritePosition();
}

void CPU::RunAdd(int inValue)
{
	for (int i = 0; i < 2; ++i)
	{
		TickCycle();
	}

	RegisterX += inValue;

	UpdateSpritePosition();
}

void CPU::RunNoop()
{
	TickCycle();
}

int CPU::GetSignalSum() const
{
	int totalSum = 0;
	for (int signalPoint : SignalPoints)
	{
		totalSum += signalPoint;
	}

	return totalSum;
}

std::string CPU::GetDeviceScreen() const
{
	std::string outString;

	for (const CharArray& screenRow : DeviceScreen)
	{
		outString.append(screenRow.data(), ScreenWidth);
		outString.push_back('\n');
	}

	return outString;
}

void CPU::TickCycle()
{
	DrawPixel();

	++CycleCounter;

	if (CycleCounter >= 20)
	{
		const int counterOffset = CycleCounter - 20;

		if (counterOffset == 0 || counterOffset % 40 == 0)
		{
			SignalPoints.push_back(CycleCounter * RegisterX);
		}
	}
}

void CPU::UpdateSpritePosition()
{
	SpriteBuffer.fill('.');

	for (int i = -1; i < 2; ++i)
	{
		int drawIndex = RegisterX + i;
		if (drawIndex >= 0 && drawIndex < ScreenWidth)
		{
			SpriteBuffer[drawIndex] = '#';
		}
	}
}

void CPU::DrawPixel()
{
	const int rowIndex = CycleCounter / 40;
	const int columnIndex = CycleCounter % 40;

	CharArray& screenRow = DeviceScreen[rowIndex];

	screenRow[columnIndex] = SpriteBuffer[columnIndex];
}

Parser::Parser()
	:DeviceCPU()
{
}

void Parser::OnStartParse()
{
}

void Parser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	AdventParser::VectorString splitString = AdventParser::SplitString(inLine, ' ');

	Instruction currentInstruction = GetInstructionFromString(splitString[0]);
	
	switch (currentInstruction)
	{
	case Instruction::Noop:
		DeviceCPU.RunNoop();
		break;
	case Instruction::Addx:
	{
		int addValue = std::stoi(splitString[1]);
		DeviceCPU.RunAdd(addValue);
	}
		break;
	default:
		break;
	}
}

void Parser::OnEndParse()
{
}

int Parser::GetSignalSum() const
{
	return DeviceCPU.GetSignalSum();
}

std::string Parser::GetDeviceScreen() const
{
	return DeviceCPU.GetDeviceScreen();
}