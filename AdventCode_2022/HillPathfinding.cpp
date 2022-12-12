#include "HillPathfinding.h"

#include <algorithm>
#include <iostream>

using namespace Hill;

HillPoint HillPoint::operator+(const HillPoint& inOtherPoint) const
{
	HillPoint out;
	out.X = X + inOtherPoint.X;
	out.Y = Y + inOtherPoint.Y;

	return out;
}

bool HillNode::operator==(const HillNode& inOtherNode) const
{
	return Point.X == inOtherNode.Point.X && Point.Y == inOtherNode.Point.Y;
}

bool HillNode::operator!=(const HillNode& inOtherNode) const
{
	return !(*this == inOtherNode);
}

bool CanMoveTo(const HillNode& inFrom, const HillNode& inTo)
{
	if (inFrom.Height < 'z')
	{
		return inFrom.Height + 1 >= inTo.Height;
	}
	else
	{
		return true;
	}
}

HillMap::HillMap()
	:Map()
	,StartPoint()
	,EndPoint()
{
}

void HillMap::Initialize(const StringVector& inMapData)
{
	Map.clear();

	if (inMapData.size() > 0)
	{
		int height = static_cast<int>(inMapData.size());
		int width = static_cast<int>(inMapData.front().size());

		if (height > 0 && width > 0)
		{
			Map.resize(width);
			for (HillVector& hillColumn : Map)
			{
				hillColumn.resize(height);
			}

			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					char height = inMapData[y][x];
					HillNode& hill = Map[x][y];

					hill.Point.X = x;
					hill.Point.Y = y;

					if (height == 'S')
					{
						hill.Height = 'a';

						StartPoint.X = x;
						StartPoint.Y = y;
					}
					else if (height == 'E')
					{
						hill.Height = 'z';
						EndPoint.X = x;
						EndPoint.Y = y;
					}
					else
					{
						hill.Height = height;
					}
				}
			}
		}

		MapPath();
	}
}

HillNode& HillMap::GetNode(const HillPoint& inPoint)
{
	return Map[inPoint.X][inPoint.Y];
}

const HillNode& HillMap::GetNode(const HillPoint& inPoint) const
{
	return Map[inPoint.X][inPoint.Y];
}

size_t HillMap::GetShortestPathLength() const
{
	const HillNode& endNode = GetNode(EndPoint);

	return endNode.DistanceFromStart;
}

void CheckNode(const HillNode& currentNode, HillNode& inNode, HillReferenceVector& outCheckedNodes)
{
	if (!inNode.Visited)
	{
		if (CanMoveTo(currentNode, inNode))
		{
			int distance = currentNode.DistanceFromStart + 1;
			if (distance < inNode.DistanceFromStart)
			{
				inNode.DistanceFromStart = distance;
			}

			outCheckedNodes.push_back(&inNode);
		}
	}
}

bool NodeSort(const HillNode* A, const HillNode* B)
{
	return A->DistanceFromStart < B->DistanceFromStart;
}

void HillMap::MapPath()
{
	const int height = static_cast<int>(Map.front().size());
	const int width = static_cast<int>(Map.size());

	HillNode* startNode = &GetNode(StartPoint);
	HillNode* endNode = &GetNode(EndPoint);

	startNode->DistanceFromStart = 0;
	startNode->Visited = true;

	HillNode* currentNode = startNode;

	HillReferenceVector checkedNodes;

	while (currentNode != endNode)
	{
		checkedNodes.clear();
		const HillPoint& currentPoint = currentNode->Point;

		// Check Up
		if (currentPoint.Y > 0)
		{
			HillNode& upNode = GetNode(currentPoint + UpOffset);

			CheckNode(*currentNode, upNode, checkedNodes);
		}

		//Check Down
		if (currentPoint.Y < (height - 1))
		{
			HillNode& downNode = GetNode(currentPoint + DownOffset);

			CheckNode(*currentNode, downNode, checkedNodes);
		}

		//Check Right
		if (currentPoint.X < (width - 1))
		{
			HillNode& rightNode = GetNode(currentPoint + RightOffset);

			CheckNode(*currentNode, rightNode, checkedNodes);
		}

		//Check Left
		if (currentPoint.X > 0)
		{
			HillNode& leftNode = GetNode(currentPoint + LeftOffset);

			CheckNode(*currentNode, leftNode, checkedNodes);
		}


		currentNode->Visited = true;
		if (checkedNodes.size() > 0)
		{
			std::sort(checkedNodes.begin(), checkedNodes.end(), NodeSort);

			currentNode = checkedNodes[0];
		}
		else
		{
			DebugDrawMap();
			break;
		}
	}
}

void HillMap::DebugDrawMap() const
{
	const int height = static_cast<int>(Map.front().size());
	const int width = static_cast<int>(Map.size());

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			const HillNode& node = Map[x][y];

			if (node.Point.X == StartPoint.X && node.Point.Y == StartPoint.Y)
			{
				std::cout << 'S';
			}
			else if (node.Point.X == EndPoint.X && node.Point.Y == EndPoint.Y)
			{
				std::cout << 'E';
			}
			else if (node.Visited)
			{
				std::cout << 'v';
			}
			else
			{
				std::cout << '.';
			}
		}
		std::cout << std::endl;
	}
}

HillParser::HillParser()
	:Map()
	,MapData()
{
}

void HillParser::OnStartParse()
{
	MapData.clear();
}

void HillParser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	MapData.push_back(inLine);
}

void HillParser::OnEndParse()
{
	Map.Initialize(MapData);
}