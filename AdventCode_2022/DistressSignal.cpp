#include "DistressSignal.h"

#include <iostream>
#include <algorithm>

using namespace Distress;

bool Data::IsValue() const
{
	return Children.size() == 0 && Value >= 0;
}

bool Data::AreEqual(const Data& A, const Data& B)
{
	const size_t sizeA = A.Children.size();
	const size_t sizeB = B.Children.size();

	const bool AIsValue = A.IsValue();
	const bool BIsValue = B.IsValue();

	if (AIsValue && BIsValue)
	{
		return A.Value == B.Value;
	}

	if (sizeA == sizeB)
	{
		if (sizeA > 0 && sizeB > 0)
		{
			for (size_t i = 0; i < sizeA; ++i)
			{
				if (!AreEqual(A.Children[i], B.Children[i]))
				{
					return false;
				}
			}
		}

		return true;
	}

	return false;
}

void Packet::Parse(const std::string& inPacketString)
{
	AdventParser::VectorString splitPacketString;

	auto AddPiece = [&splitPacketString](const char inPiece)
	{
		std::string piece;
		piece += inPiece;
		splitPacketString.push_back(piece);
	};

	auto AppendNumber = [&splitPacketString](const char inNumberChar)
	{
		splitPacketString.back() += inNumberChar;
	};

	for (const char c : inPacketString)
	{
		switch (c)
		{
		case '[':
		case ']':
		case ',':
			AddPiece(c);
			break;
		default:
		{
			std::string& lastString = splitPacketString.back();
			if (lastString == "[" || lastString == ",")
			{
				AddPiece(c);
			}
			else
			{
				AppendNumber(c);
			}
		}
			break;
		}
	}

	Data* currentDataList = nullptr;
	for (const std::string& piece : splitPacketString)
	{
		if (piece == "[")
		{
			if (currentDataList == nullptr)
			{
				currentDataList = &DataList;
			}
			else
			{
				currentDataList->Children.emplace_back();
				Data& newData = currentDataList->Children.back();
				newData.Parent = currentDataList;
				currentDataList = &newData;
			}
		}
		else if (piece == "]")
		{
			if (currentDataList != nullptr)
			{
				if (currentDataList->Parent != nullptr)
				{
					currentDataList = currentDataList->Parent;
				}
			}
		}
		else
		{
			if (piece != ",")
			{
				int value = std::stoi(piece);
				if (currentDataList != nullptr)
				{
					currentDataList->Children.push_back(Data{ value });
				}
			}
		}
	}

	Parsed = true;
}

OrderType PacketPair::CompareData(const Data& leftData, const Data& rightData)
{
	const bool leftDataIsValue = leftData.IsValue();
	const bool rightDataIsValue = rightData.IsValue();
	if (leftDataIsValue && rightDataIsValue)
	{
		if (leftData.Value == rightData.Value)
		{
			return OrderType::Inconclusive;
		}
		else if (leftData.Value < rightData.Value)
		{
			return OrderType::InOrder;
		}
		else
		{
			return OrderType::OutOfOrder;
		}
	}

	if (!leftDataIsValue && !rightDataIsValue)
	{
		const size_t leftSize = leftData.Children.size();
		const size_t rightSize = rightData.Children.size();

		if (leftSize > 0 && rightSize > 0)
		{
			for (size_t i = 0; i < leftSize; ++i)
			{
				if (i >= rightSize)
				{
					return OrderType::OutOfOrder;
				}

				OrderType result = CompareData(leftData.Children[i], rightData.Children[i]);
				if (result != OrderType::Inconclusive)
				{
					return result;
				}
			}
		}

		if (leftSize != rightSize)
		{
			if (leftSize < rightSize)
			{
				return OrderType::InOrder;
			}
			else
			{
				return OrderType::OutOfOrder;
			}
		}
	}

	if (leftDataIsValue && !rightDataIsValue)
	{
		Data leftValueList;
		leftValueList.Children.push_back(Data {leftData.Value});

		return CompareData(leftValueList, rightData);
	}

	if (!leftDataIsValue && rightDataIsValue)
	{
		Data rightValueList;
		rightValueList.Children.push_back(Data {rightData.Value});

		return CompareData(leftData, rightValueList);
	}

	return OrderType::Inconclusive;
}

bool PacketPair::IsInCorrectOrder() const
{
	const Data& leftData = Left.DataList;
	const Data& rightData = Right.DataList;

	return CompareData(leftData, rightData) == OrderType::InOrder;
}

DistressSignalParser::DistressSignalParser()
	:PacketPairs()
{
}

void DistressSignalParser::OnStartParse()
{
	std::cout << "Attempting to get signal in order..." << std::endl;
	PacketPairs.clear();

	PacketPairs.emplace_back();

}

void DistressSignalParser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	if (!inLine.empty())
	{
		PacketPair& currentPair = PacketPairs.back();

		if (!currentPair.Left.HasParsed())
		{
			currentPair.Left.Parse(inLine);
		}
		else
		{
			currentPair.Right.Parse(inLine);
		}
	}
	else
	{
		PacketPairs.emplace_back();
	}
}

void DistressSignalParser::OnEndParse()
{
	CheckOrder();
	FindDecoderKey();
}

void DistressSignalParser::CheckOrder() const
{
	const size_t numPairs = PacketPairs.size();
	size_t sumofIndices = 0;
	size_t numCorrect = 0;

	for (size_t i = 0; i < numPairs; ++i)
	{
		const PacketPair& packetPair = PacketPairs[i];
		if (packetPair.IsInCorrectOrder())
		{
			sumofIndices += (i + 1);
			++numCorrect;
		}
	}

	std::cout << "Parsed signal and the sum of indices of packets in order was " << sumofIndices << std::endl;
}

void DistressSignalParser::FindDecoderKey() const
{
	std::vector<Packet> packets;

	Packet firstDivider;
	firstDivider.Parse("[[2]]");
	Packet secondDivider;
	secondDivider.Parse("[[6]]");

	packets.push_back(firstDivider);
	packets.push_back(secondDivider);

	for (const PacketPair& pair : PacketPairs)
	{
		packets.push_back(pair.Left);
		packets.push_back(pair.Right);
	}

	auto packetSort = [](const Packet& left, const Packet& right) -> bool
	{
		return PacketPair::CompareData(left.DataList, right.DataList) == OrderType::InOrder;
	};

	std::sort(packets.begin(), packets.end(), packetSort);

	size_t firstDividerIndex = 0;
	size_t secondDividerIndex = 0;
	for (size_t i = 0; i < packets.size(); ++i)
	{
		const Packet& packet = packets[i];

		if (Data::AreEqual(packet.DataList, firstDivider.DataList))
		{
			firstDividerIndex = i + 1;
		}

		if (Data::AreEqual(packet.DataList, secondDivider.DataList))
		{
			secondDividerIndex = i + 1;
		}
	}

	size_t decoderKey = firstDividerIndex * secondDividerIndex;

	std::cout << "Decoder key is " << decoderKey << std::endl;
}