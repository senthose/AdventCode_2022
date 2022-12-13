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
	,ShortestPath()
	,ShortestScenicPath()
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

		MapPaths();
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
	return ShortestPath;
}

size_t HillMap::GetShortestScenicPathLength() const
{
	return ShortestScenicPath;
}

void CheckNode(const HillNode& currentNode, HillNode& inNode, HillReferenceVector& outCheckedNodes)
{
	size_t startSize = outCheckedNodes.size();
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

void AddUnvisited(HillNode* inNode, HillReferenceVector& outUnvisitedNodes)
{
	if (!inNode->Visited)
	{
		bool foundNode = false;
		for (const HillNode* node : outUnvisitedNodes)
		{
			if (inNode == node)
			{
				foundNode = true;
				break;
			}
		}

		if (!foundNode)
		{
			outUnvisitedNodes.push_back(inNode);
		}
	}
}

void RemoveVisited(HillReferenceVector& outUnvistedNodes)
{
	auto FindVisited = [](const HillNode* inNode)
	{
		return inNode->Visited;
	};

	HillReferenceVector::iterator it = std::find_if(outUnvistedNodes.begin(), outUnvistedNodes.end(), FindVisited);

	while (it != outUnvistedNodes.end())
	{
		outUnvistedNodes.erase(it);

		it = std::find_if(outUnvistedNodes.begin(), outUnvistedNodes.end(), FindVisited);
	}
}

bool NodeSort(const HillNode* A, const HillNode* B)
{
	return A->DistanceFromStart < B->DistanceFromStart;
}

void HillMap::Reset()
{
	for (HillVector& mapColumn : Map)
	{
		for (HillNode& node : mapColumn)
		{
			node.Visited = false;
			node.DistanceFromStart = std::numeric_limits<int>::max();
		}
	}
}

bool HillMap::FoundEnd() const
{
	const HillNode& endNode = GetNode(EndPoint);

	return endNode.Visited;
}

void HillMap::CheckNodes(HillNode* inNode, int inHeight, int inWidth, HillReferenceVector& outUnvisitedNodes)
{
	if (FoundEnd())
	{
		return;
	}

	HillReferenceVector checkedNodes;

	const HillPoint& currentPoint = inNode->Point;

	// Check Up
	if (currentPoint.Y > 0)
	{
		HillNode& upNode = GetNode(currentPoint + UpOffset);

		CheckNode(*inNode, upNode, checkedNodes);
	}

	//Check Down
	if (currentPoint.Y < (inHeight - 1))
	{
		HillNode& downNode = GetNode(currentPoint + DownOffset);

		CheckNode(*inNode, downNode, checkedNodes);
	}

	//Check Right
	if (currentPoint.X < (inWidth - 1))
	{
		HillNode& rightNode = GetNode(currentPoint + RightOffset);

		CheckNode(*inNode, rightNode, checkedNodes);
	}

	//Check Left
	if (currentPoint.X > 0)
	{
		HillNode& leftNode = GetNode(currentPoint + LeftOffset);

		CheckNode(*inNode, leftNode, checkedNodes);
	}

	inNode->Visited = true;

	if (FoundEnd())
	{
		return;
	}

	if (checkedNodes.size() > 0)
	{
		for (HillNode* node : checkedNodes)
		{
			AddUnvisited(node, outUnvisitedNodes);
		}
	}
}

void HillMap::MapPath(HillNode* inStartPoint)
{
	Reset();

	const int height = static_cast<int>(Map.front().size());
	const int width = static_cast<int>(Map.size());

	HillNode* startNode = inStartPoint;

	startNode->DistanceFromStart = 0;
	startNode->Visited = true;

	HillReferenceVector unvistedNodes;

	HillNode* currentNode = startNode;

	while (!FoundEnd())
	{
		CheckNodes(currentNode, height, width, unvistedNodes);

		RemoveVisited(unvistedNodes);

		if (unvistedNodes.size() > 0)
		{
			std::sort(unvistedNodes.begin(), unvistedNodes.end(), NodeSort);

			currentNode = unvistedNodes[0];
		}
		else
		{
			break;
		}
	}
}

void HillMap::MapPaths()
{
	ShortestScenicPath = std::numeric_limits<size_t>::max();
	const HillNode& endNode = GetNode(EndPoint);

	for (HillVector& mapColumn : Map)
	{
		for (HillNode& node : mapColumn)
		{
			if (node.Height == 'a')
			{
				MapPath(&node);

				if (endNode.DistanceFromStart < ShortestScenicPath)
				{
					ShortestScenicPath = endNode.DistanceFromStart;
				}

				if (node.Point.X == StartPoint.X && node.Point.Y == StartPoint.Y)
				{
					ShortestPath = endNode.DistanceFromStart;
				}
			}
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

size_t HillParser::GetShortestPath() const
{
	return Map.GetShortestPathLength();
}

size_t HillParser::GetShortestScenicPath() const
{
	return Map.GetShortestScenicPathLength();
}