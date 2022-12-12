#include "Monkey.h"

#include <algorithm>
#include <iostream>

using namespace MonkeyBusiness;

OperationType GetOperationType(const char inOperationChar)
{
	switch (inOperationChar)
	{
	case '+':
		return OperationType::Add;
	case '*':
		return OperationType::Multiply;
	default:
		return OperationType::Invalid;
	}
}

void MonkeyOperation::SetOperation(const std::string& inLeft, const std::string& inOpString, const std::string& inRight)
{
	bool leftValueIsOld = false;
	bool rightValueIsOld = false;

	BigValue leftValue = 0;
	BigValue rightValue = 0;

	OperationType parsedOperation = GetOperationType(inOpString[0]);

	if (inLeft == "old")
	{
		leftValueIsOld = true;
	}
	else
	{
		leftValue = std::stoull(inLeft);
	}

	if (inRight == "old")
	{
		rightValueIsOld = true;
	}
	else
	{
		rightValue = std::stoull(inRight);
	}

	CachedFunction = [parsedOperation, leftValueIsOld, rightValueIsOld, leftValue, rightValue](BigValue inOld) -> BigValue
	{
		BigValue left = leftValueIsOld ? inOld : leftValue;
		BigValue right = rightValueIsOld ? inOld : rightValue;

		switch (parsedOperation)
		{
		case OperationType::Add:
			return left + right;
		case OperationType::Multiply:
			return left * right;
		default:
			return 0;
		}
	};
}

BigValue MonkeyOperation::operator()(BigValue inOld)
{
	BigValue returnValue = 0;
	if (CachedFunction)
	{
		returnValue = CachedFunction(inOld);
	}

	return returnValue;
}

Monkey::Monkey()
	:HeldItems()
	, Operation()
	,TestValue(0)
	,TrueTarget(-1)
	,FalseTarget(-1)
	,NumItemsInspected(0)
{
}

void Monkey::AddItem(BigValue inWorryValue)
{
	HeldItems.push_back(inWorryValue);
}

void Monkey::SetOperation(const std::string& inLeft, const std::string& inOpString, const std::string& inRight)
{
	Operation.SetOperation(inLeft, inOpString, inRight);
}

void Monkey::SetTestValue(int inTestValue)
{
	TestValue = inTestValue;
}

void Monkey::SetTrueTarget(int inTarget)
{
	TrueTarget = inTarget;
}

void Monkey::SetFalseTarget(int inTarget)
{
	FalseTarget = inTarget;
}

void Monkey::TakeTurn(ThrowVector& outThrows, bool inWorried)
{
	outThrows.clear();

	for (BigValue itemValue : HeldItems)
	{
		BigValue worry = Operation(itemValue);
		if (worry < itemValue)
		{
			//std::cout << "OVERFLOW" << std::endl;
		}

		if (inWorried)
		{
			worry /= 3;
		}
		else
		{
			// JC did this (Was the least common demoniator of all the tests)
			worry %= 9699690;
		}


		if (worry % TestValue == 0)
		{
			outThrows.push_back({ TrueTarget, worry });
		}
		else
		{
			outThrows.push_back({ FalseTarget, worry });
		}


		BigValue oldInspect = NumItemsInspected;
		++NumItemsInspected;
		if (NumItemsInspected < oldInspect)
		{
			std::cout << "other OVERFLOW" << std::endl;
		}
	}

	HeldItems.clear();
}

MonkeyGroup::MonkeyGroup()
	:Monkeys()
{
}

void MonkeyGroup::AddMonkey()
{
	Monkeys.emplace_back();
}

Monkey& MonkeyGroup::GetMonkey(int inMonkeyIndex)
{
	return Monkeys[inMonkeyIndex];
}

void MonkeyGroup::PerformRound(bool inWorried)
{
	ThrowVector throws;

	for (Monkey& monkey : Monkeys)
	{
		monkey.TakeTurn(throws, inWorried);

		for (const MonkeyThrow & monkeyThrow : throws)
		{
			Monkeys[monkeyThrow.TargetMonkey].AddItem(monkeyThrow.ItemValue);
		}
	}
}

BigValue MonkeyGroup::GetCurrentMonkeyBusinessValue() const
{
	std::vector<BigValue> interactions;

	for (const Monkey& monkey : Monkeys)
	{
		interactions.push_back(monkey.GetNumItemsInspected());
	}

	auto interactionSort = [](BigValue A, BigValue B) -> bool
	{
		return A > B;
	};

	std::sort(interactions.begin(), interactions.end(), interactionSort);

	return interactions[0] * interactions[1];
}

MonkeyParser::MonkeyParser()
	:Monkeys()
	,CurrentMonkeyBeingParsed(-1)
	,Worried(true)
{
}

void MonkeyParser::OnStartParse()
{
}

void MonkeyParser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	if (!inLine.empty())
	{
		if (CurrentMonkeyBeingParsed == -1)
		{
			AdventParser::VectorString splitString = AdventParser::SplitString(inLine, ' ');

			if (splitString[0] == "Monkey")
			{
				std::string& numberString = splitString[1];

				AdventParser::RemoveCharFromString(numberString, ':');

				int monkeyIndex = std::stoi(numberString);

				Monkeys.AddMonkey();
				CurrentMonkeyBeingParsed = monkeyIndex;
			}
		}
		else
		{
			Monkey& currentMonkey = Monkeys.GetMonkey(CurrentMonkeyBeingParsed);
			AdventParser::VectorString splitString = AdventParser::SplitString(inLine, ' ');

			if (splitString[0] == "Starting")
			{
				size_t numSegments = splitString.size();
				if (numSegments > 2)
				{
					for (size_t i = 2; i < numSegments; ++i)
					{
						std::string& itemString = splitString[i];
						AdventParser::RemoveCharFromString(itemString, ',');

						int itemValue = std::stoi(itemString);

						currentMonkey.AddItem(itemValue);
					}
				}
			}

			if (splitString[0] == "Operation:")
			{
				currentMonkey.SetOperation(splitString[3], splitString[4], splitString[5]);
			}

			if (splitString[0] == "Test:")
			{
				int testValue = std::stoi(splitString[3]);
				currentMonkey.SetTestValue(testValue);
			}

			if (splitString[0] == "If")
			{
				if (splitString[1] == "true:")
				{
					int trueTarget = std::stoi(splitString[5]);
					currentMonkey.SetTrueTarget(trueTarget);
				}

				if (splitString[1] == "false:")
				{
					int falseTarget = std::stoi(splitString[5]);
					currentMonkey.SetFalseTarget(falseTarget);
				}
			}
		}
	}
	else
	{
		CurrentMonkeyBeingParsed = -1;
	}
}

void MonkeyParser::OnEndParse()
{
}

void MonkeyParser::DoRounds(BigValue inNumRounds)
{
	for (BigValue i = 0; i < inNumRounds; ++i)
	{
		Monkeys.PerformRound(Worried);
	}
}

BigValue MonkeyParser::GetMonkeyBusinessValue() const
{
	return Monkeys.GetCurrentMonkeyBusinessValue();
}