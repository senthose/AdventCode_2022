#include "Rope.h"

using namespace Rope;

int Clamp(int inValue, int min, int max)
{
	if (inValue < min)
	{
		return min;
	}

	if (inValue > max)
	{
		return max;
	}

	return inValue;
}

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

void MovePoint(MoveDirection inDirection, int& outX, int& outY)
{
	switch (inDirection)
	{
	case MoveDirection::North:
		--outY;
		break;
	case MoveDirection::South:
		++outY;
		break;
	case MoveDirection::East:
		++outX;
		break;
	case MoveDirection::West:
		--outX;
		break;
	default:
		break;
	}
}

void MovePoint(MoveDirection inDirection, RopePoint& outPoint)
{
	MovePoint(inDirection, outPoint.X, outPoint.Y);
}

RopeNode& GetRopeNode(RopeVector2D& inGrid, int inHalfWidth, int x, int y)
{
	return inGrid[static_cast<size_t>(inHalfWidth) + x][static_cast<size_t>(inHalfWidth) + y];
}

RopeNode& GetRopeNode(RopeVector2D& inGrid, int inHalfWidth, const RopePoint& inPoint)
{
	return GetRopeNode(inGrid, inHalfWidth, inPoint.X, inPoint.Y);
}

void RopeNode::SetVisited(size_t inRopeIndex)
{
	VisitBits.set(inRopeIndex);
}

bool RopeNode::WasVisited(size_t inRopeIndex) const
{
	return VisitBits.test(inRopeIndex);
}

RopeMap::RopeMap()
	:RopeGrid()
	,RopePoints()
	,HalfWidth(0)
{
	SetGridSize(50);

	RopeNode& node = GetRopeNode(RopeGrid, HalfWidth, 0, 0);

	for (int i = 0; i < NumRopeSegments; ++i)
	{
		node.SetVisited(i);
	}
}

void RopeMap::MoveHead(MoveDirection inDirection)
{
	RopePoint& headPoint = RopePoints[0];

	MovePoint(inDirection, headPoint);
	if (!IsValidCoord(headPoint))
	{
		SetGridSize(HalfWidth * 2);
	}

	GetRopeNode(RopeGrid, HalfWidth, headPoint).SetVisited(0);

	bool lastSegmentMoved = true;
	for (size_t i = 1; i < NumRopeSegments; ++i)
	{
		if (!lastSegmentMoved)
		{
			break;
		}

		const RopePoint& lastSegment = RopePoints[i - 1];
		RopePoint& tailSegment = RopePoints[i];
		bool isTailClose = false;
		for (int x = -1; x <= 1; ++x)
		{
			int checkX = lastSegment.X + x;
			for (int y = -1; y <= 1; ++y)
			{
				int checkY = lastSegment.Y + y;

				if (checkX == tailSegment.X && checkY == tailSegment.Y)
				{
					isTailClose = true;
					break;
				}
			}
		}

		if (!isTailClose)
		{
			int xDif = lastSegment.X - tailSegment.X;
			int yDif = lastSegment.Y - tailSegment.Y;

			xDif = Clamp(xDif, -1, 1);
			yDif = Clamp(yDif, -1, 1);
			
			tailSegment.X += xDif;
			tailSegment.Y += yDif;
			
			GetRopeNode(RopeGrid, HalfWidth, tailSegment).SetVisited(i);
		}
		else
		{
			lastSegmentMoved = false;
		}
	}
}

size_t RopeMap::GetNumTailSpaces(size_t inTailIndex) const
{
	size_t numTailSpaces = 0;
	for (const RopeVector& gridColumn : RopeGrid)
	{
		for (const RopeNode& node : gridColumn)
		{
			if (node.WasVisited(inTailIndex))
			{
				++numTailSpaces;
			}
		}
	}

	return numTailSpaces;
}

bool RopeMap::IsValidCoord(int x, int y) const 
{
	return (HalfWidth > std::abs(x)) && (HalfWidth > std::abs(y));
}

bool RopeMap::IsValidCoord(const RopePoint& inPoint) const
{
	return IsValidCoord(inPoint.X, inPoint.Y);
}

void RopeMap::SetGridSize(int newHalfWidth)
{
	if (HalfWidth < newHalfWidth)
	{
		int oldHalfWidth = HalfWidth;
		HalfWidth = newHalfWidth;

		RopeVector2D oldGrid = RopeGrid;

		RopeGrid.clear();
		RopeGrid.resize((static_cast<size_t>(HalfWidth) * 2) + 1);
		for (RopeVector& gridColumn : RopeGrid)
		{
			gridColumn.resize((static_cast<size_t>(HalfWidth) * 2) + 1);
		}

		for (int i = -oldHalfWidth + 1; i < oldHalfWidth; ++i)
		{
			for (int j = -oldHalfWidth + 1; j < oldHalfWidth; ++j)
			{
				GetRopeNode(RopeGrid, HalfWidth, i, j) = GetRopeNode(oldGrid, oldHalfWidth, i, j);
			}
		}
	}
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

size_t RopeParser::GetNumTailSpaces(size_t inTailIndex) const
{
	return Map.GetNumTailSpaces(inTailIndex);
}