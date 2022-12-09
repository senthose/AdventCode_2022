#include "Rope.h"

using namespace Rope;

MoveDirection GetDirectionFromChar(const char inChar)
{
	switch (inChar)
	{
	case 'U':
		return MoveDirection::North;
	case 'D':
		return MoveDirection::South;
	case 'L':
		return MoveDirection::West;
	case 'R':
		return MoveDirection::East;
	default:
		return MoveDirection::Size;
	}
}

MoveDirection GetOppositDirection(MoveDirection inDirection)
{
	switch (inDirection)
	{
	case MoveDirection::North:
		return MoveDirection::South;
	case MoveDirection::South:
		return MoveDirection::North;
	case MoveDirection::East:
		return MoveDirection::West;
	case MoveDirection::West:
		return MoveDirection::East;
	case MoveDirection::NorthEast:
		return MoveDirection::SouthWest;
	case MoveDirection::NorthWest:
		return MoveDirection::SouthEast;
	case MoveDirection::SouthEast:
		return MoveDirection::NorthWest;
	case MoveDirection::SouthWest:
		return MoveDirection::NorthEast;
	case MoveDirection::Size:
	default:
		return inDirection;
	}
}

RopeNode::RopeNode()
	:Directions()
{
	for (size_t i = 0; i < static_cast<size_t>(MoveDirection::Size); ++i)
	{
		Directions[i] = nullptr;
	}
}

RopeNode* RopeNode::GetNode(MoveDirection inDirection)
{
	return Directions[static_cast<size_t>(inDirection)];
}

const RopeNode* RopeNode::GetNode(MoveDirection inDirection) const
{
	return Directions[static_cast<size_t>(inDirection)];
}

void RopeNode::SetNode(MoveDirection inDirection, RopeNode* inNode)
{
	Directions[static_cast<size_t>(inDirection)] = inNode;
}

RopeMap::RopeMap()
	:Nodes()
	,HeadPosition(nullptr)
	,TailPosition(nullptr)
	,StartPosition(nullptr)
{
	RopeNode* StartNode = CreateNode(MoveDirection::Size, nullptr);

	StartNode->TailEntered = true;
	StartPosition = StartNode;
	HeadPosition = StartNode;
	TailPosition = StartNode;
}

RopeNode* RopeMap::MakeNode()
{
	Nodes.emplace_front();
	RopeNode* newNode = &Nodes.front();
	return newNode;
}

RopeNode* RopeMap::CreateNode(MoveDirection fromDirection, RopeNode* inAdjacentNode)
{
	RopeNode* newNode = MakeNode();

	if (fromDirection != MoveDirection::Size)
	{
		MoveDirection oppositeDirection = GetOppositDirection(fromDirection);

		newNode->SetNode(oppositeDirection, inAdjacentNode);

		switch (oppositeDirection)
		{
		case Rope::MoveDirection::North:
			newNode->SetNode(MoveDirection::NorthEast, inAdjacentNode->GetNode(MoveDirection::SouthEast));
			newNode->SetNode(MoveDirection::NorthWest, inAdjacentNode->GetNode(MoveDirection::SouthWest));
		case Rope::MoveDirection::South:
			newNode->SetNode(MoveDirection::SouthEast, inAdjacentNode->GetNode(MoveDirection::NorthEast));
			newNode->SetNode(MoveDirection::SouthWest, inAdjacentNode->GetNode(MoveDirection::NorthWest));
		case Rope::MoveDirection::East:
			newNode->SetNode(MoveDirection::NorthEast, inAdjacentNode->GetNode(MoveDirection::NorthWest));
			newNode->SetNode(MoveDirection::SouthEast, inAdjacentNode->GetNode(MoveDirection::SouthWest));
		case Rope::MoveDirection::West:
			newNode->SetNode(MoveDirection::NorthWest, inAdjacentNode->GetNode(MoveDirection::NorthEast));
			newNode->SetNode(MoveDirection::SouthWest, inAdjacentNode->GetNode(MoveDirection::SouthEast));
		case Rope::MoveDirection::NorthEast:
			break;
		case Rope::MoveDirection::NorthWest:
			break;
		case Rope::MoveDirection::SouthEast:
			break;
		case Rope::MoveDirection::SouthWest:
			break;
		}
	}

	for (size_t i = 0; i < static_cast<size_t>(MoveDirection::Size); ++i)
	{
		if (newNode->Directions[i] == nullptr)
		{
			newNode->Directions[i] = MakeNode();
		}
	}

	for (size_t i = 0; i < static_cast<size_t>(MoveDirection::Size); ++i)
	{
		MoveDirection direction = static_cast<MoveDirection>(i);
		RopeNode* node = newNode->GetNode(direction);

		switch (direction)
		{
		case Rope::MoveDirection::North:
			node->SetNode(MoveDirection::South, newNode);
			node->SetNode(MoveDirection::East, newNode->GetNode(MoveDirection::NorthEast));
			node->SetNode(MoveDirection::West, newNode->GetNode(MoveDirection::NorthWest));
			node->SetNode(MoveDirection::SouthEast, newNode->GetNode(MoveDirection::East));
			node->SetNode(MoveDirection::SouthWest, newNode->GetNode(MoveDirection::West));
			break;
		case Rope::MoveDirection::South:
			node->SetNode(MoveDirection::North, newNode);
			node->SetNode(MoveDirection::East, newNode->GetNode(MoveDirection::SouthEast));
			node->SetNode(MoveDirection::West, newNode->GetNode(MoveDirection::SouthWest));
			node->SetNode(MoveDirection::NorthEast, newNode->GetNode(MoveDirection::East));
			node->SetNode(MoveDirection::NorthWest, newNode->GetNode(MoveDirection::West));
			break;
		case Rope::MoveDirection::East:
			node->SetNode(MoveDirection::West, newNode);
			node->SetNode(MoveDirection::North, newNode->GetNode(MoveDirection::NorthEast));
			node->SetNode(MoveDirection::South, newNode->GetNode(MoveDirection::SouthEast));
			node->SetNode(MoveDirection::NorthWest, newNode->GetNode(MoveDirection::North));
			node->SetNode(MoveDirection::SouthWest, newNode->GetNode(MoveDirection::South));
			break;
		case Rope::MoveDirection::West:
			node->SetNode(MoveDirection::East, newNode);
			node->SetNode(MoveDirection::North, newNode->GetNode(MoveDirection::NorthWest));
			node->SetNode(MoveDirection::South, newNode->GetNode(MoveDirection::SouthWest));
			node->SetNode(MoveDirection::NorthEast, newNode->GetNode(MoveDirection::North));
			node->SetNode(MoveDirection::SouthEast, newNode->GetNode(MoveDirection::South));
			break;
		case Rope::MoveDirection::NorthEast:
			node->SetNode(MoveDirection::SouthWest, newNode);
			node->SetNode(MoveDirection::West, newNode->GetNode(MoveDirection::North));
			node->SetNode(MoveDirection::South, newNode->GetNode(MoveDirection::East));
			break;
		case Rope::MoveDirection::NorthWest:
			node->SetNode(MoveDirection::SouthEast, newNode);
			node->SetNode(MoveDirection::East, newNode->GetNode(MoveDirection::North));
			node->SetNode(MoveDirection::South, newNode->GetNode(MoveDirection::West));
			break;
		case Rope::MoveDirection::SouthEast:
			node->SetNode(MoveDirection::NorthWest, newNode);
			node->SetNode(MoveDirection::North, newNode->GetNode(MoveDirection::East));
			node->SetNode(MoveDirection::West, newNode->GetNode(MoveDirection::South));
			break;
		case Rope::MoveDirection::SouthWest:
			node->SetNode(MoveDirection::NorthEast, newNode);
			node->SetNode(MoveDirection::North, newNode->GetNode(MoveDirection::West));
			node->SetNode(MoveDirection::East, newNode->GetNode(MoveDirection::South));
			break;
		case Rope::MoveDirection::Size:
			break;
		default:
			break;
		}
	}
	

	return newNode;
}

RopeNode* RopeMap::GetNodeInDirection(MoveDirection inDirection, RopeNode* inCurrentNode)
{
	RopeNode* node = inCurrentNode->GetNode(inDirection);

	if (node == nullptr)
	{
		node = CreateNode(inDirection, inCurrentNode);
	}

	return node;
}

void RopeMap::MoveHead(MoveDirection inDirection)
{
	RopeNode* oldHeadPosition = HeadPosition;
	RopeNode* newHeadPosition = GetNodeInDirection(inDirection, HeadPosition);

	HeadPosition = newHeadPosition;

	if (TailPosition != HeadPosition)
	{
		bool tailAdjacent = false;

		for (RopeNode* node : HeadPosition->Directions)
		{
			if (node == TailPosition)
			{
				tailAdjacent = true;
				break;
			}
		}

		if (!tailAdjacent)
		{
			TailPosition = oldHeadPosition;
			TailPosition->TailEntered = true;
		}
	}
}

size_t RopeMap::GetNumTailSpaces() const
{
	size_t numTailSpaces = 0;
	for (const RopeNode& node : Nodes)
	{
		if (node.TailEntered)
		{
			++numTailSpaces;
		}
	}

	return numTailSpaces;
}

RopeParser::RopeParser()
	:Map()
{
}

void RopeParser::OnStartParse()
{
}

void RopeParser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	AdventParser::VectorString splitString = AdventParser::SplitString(inLine, ' ');

	MoveDirection direction = GetDirectionFromChar(splitString[0][0]);
	size_t numSteps = std::stoull(splitString[1]);

	for (size_t i = 0; i < numSteps; ++i)
	{
		Map.MoveHead(direction);
	}
}

void RopeParser::OnEndParse()
{
}

size_t RopeParser::GetNumTailSpaces() const
{
	return Map.GetNumTailSpaces();
}