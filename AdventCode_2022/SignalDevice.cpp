#include "SignalDevice.h"

using namespace SignalDevice;

SignalDeviceParser::SignalDeviceParser()
	:StartOfBuffer()
	,StartOfRealBuffer()
{
}

void SignalDeviceParser::OnStartParse()
{
}

bool AllUniqueCharacters(const char* inChars, size_t inLength)
{
	std::string foundCharacters;
	for (size_t j = 0; j < inLength; ++j)
	{
		const char letter = inChars[j];
		if (foundCharacters.find(letter) == std::string::npos)
		{
			foundCharacters.push_back(letter);
		}
	}

	return foundCharacters.size() == inLength;
}

void SignalDeviceParser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	const size_t signalLength = 4;
	const size_t realSignalLength = 14;
	const size_t lineLength = inLine.size();

	bool foundShortSignalIndex = false;
	bool foundRealSignalIndex = false;
	if (!inLine.empty() && lineLength > realSignalLength)
	{
		for (size_t i = 3; i < lineLength; ++i)
		{

			if (!foundShortSignalIndex)
			{
				const char* lineSegment = &inLine[i - (signalLength - 1)];
				if (AllUniqueCharacters(lineSegment, signalLength))
				{
					StartOfBuffer = i + 1;
					foundShortSignalIndex = true;
				}
			}

			if (foundShortSignalIndex && !foundRealSignalIndex && i >= realSignalLength)
			{
				const char* lineSegment = &inLine[i - (realSignalLength - 1)];
				if (AllUniqueCharacters(lineSegment, realSignalLength))
				{
					StartOfRealBuffer = i + 1;
					foundRealSignalIndex = true;
				}
			}

			if (foundShortSignalIndex && foundRealSignalIndex)
			{
				break;
			}
		}
	}
}

void SignalDeviceParser::OnEndParse()
{
}