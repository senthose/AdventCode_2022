#include "Sand.h"

#include <limits>
#include <iostream>

using namespace Sand;

CavePoint CavePoint::Parse(const std::string& inPointString)
{
	AdventParser::VectorString splitString = AdventParser::SplitString(inPointString, ',');

	const int x = std::stoi(splitString[0]);
	const int y = std::stoi(splitString[1]);

	return CavePoint {x, y};
}

bool CavePoint::operator<(const CavePoint& inOther) const
{
	if (Y == inOther.Y)
	{
		return X < inOther.X;
	}
	else
	{
		return Y < inOther.Y;
	}
}

bool CavePoint::operator==(const CavePoint& inOther) const
{
	return X == inOther.X && Y == inOther.Y;
}

bool CavePoint::operator!=(const CavePoint& inOther) const
{
	return !(*this == inOther);
}

CavePoint CavePoint::operator-(const CavePoint& inRight) const
{
	return CavePoint{ X-inRight.X, Y-inRight.Y };
}

CavePoint CavePoint::operator+(const CavePoint& inRight) const
{
	return CavePoint{ X+inRight.X, Y+inRight.Y };
}

CavePoint& CavePoint::operator+=(const CavePoint& inOther)
{
	X += inOther.X;
	Y += inOther.Y;
	return *this;
}

const char CaveObject::ToChar() const
{
	switch (Type)
	{
	case Sand::CaveObjectType::Air:
		return '.';
	case Sand::CaveObjectType::Rock:
		return '#';
	case Sand::CaveObjectType::Sand:
		return 'O';
	case Sand::CaveObjectType::SandSpawner:
		return '+';
	default:
		return '?';
	}
}

CaveMap::CaveMap()
	:CaveObjects()
	,LowestDepth(0)
	,FurthestRight(0)
	,FurthestLeft(std::numeric_limits<int>::max())
	,HasFloor(false)
{
	CaveObject& sandSpawner = GetCaveObject(CavePoint{500, 0});
	sandSpawner.Type = CaveObjectType::SandSpawner;
}

void CaveMap::ParseRocks(const std::string& inWallString)
{
	std::string copy = inWallString;

	AdventParser::RemoveCharFromString(copy, '>');

	AdventParser::VectorString splitString = AdventParser::SplitString(copy, '-');

	bool hasLastPoint = false;
	CavePoint lastPoint;
	
	for (const std::string& wallPointString : splitString)
	{
		CavePoint point = CavePoint::Parse(wallPointString);

		CaveObject& object = GetCaveObject(point);
		object.Type = CaveObjectType::Rock;

		if (hasLastPoint)
		{
			AddRocks(lastPoint, point);
		}
		else
		{
			hasLastPoint = true;
		}
		lastPoint = point;

		UpdateExtents(point);
	}
}

void CaveMap::SimulateSand()
{
	const CaveObject& sandSpawner = GetCaveObject(CavePoint{500, 0});

	int numSands = 0;
	while (DropSand(sandSpawner))
	{
		++numSands;
	}

	if (HasFloor)
	{
		std::cout << "Dropped " << numSands + 1 << " sands to fill the cave" << std::endl;
	}
	else
	{
		std::cout << "Dropped " << numSands << " sands until the rest started to fall to the abyss" << std::endl;
	}
}

void CaveMap::ClearSand()
{
	for (CaveMapTree::value_type& mapPair : CaveObjects)
	{
		CaveObject& object = mapPair.second;
		if (object.Type == CaveObjectType::Sand)
		{
			object.Type = CaveObjectType::Air;
		}
	}
}

void CaveMap::AddFloor()
{
	HasFloor = true;
}

void CaveMap::DebugPrintMap() const
{
	int lowest = LowestDepth;
	if (HasFloor)
	{
		lowest += 2;
	}

	for (int y = 0; y <= lowest; ++y)
	{
		for (int x = FurthestLeft; x <= FurthestRight; ++x)
		{
			CaveMapTree::const_iterator it = CaveObjects.find(CavePoint{ x, y });
			if (it != CaveObjects.end())
			{
				const CaveObject& object = it->second;
				std::cout << object.ToChar();
			}
			else
			{
				std::cout << '.';
			}
		}

		std::cout << std::endl;
	}
}

void CaveMap::AddRocks(const CavePoint& inFrom, const CavePoint& inTo)
{
	CavePoint offset = inTo - inFrom;

	if (offset.X == 0)
	{
		if (offset.Y > 0)
		{
			offset = DownOffset;
		}
		else
		{
			offset = UpOffset;
		}
	}
	else
	{
		if (offset.X > 0)
		{
			offset = RightOffset;
		}
		else
		{
			offset = LeftOffset;
		}
	}

	CavePoint currentPoint = inFrom + offset;
	while (currentPoint != inTo)
	{
		CaveObject& object = GetCaveObject(currentPoint);
		object.Type = CaveObjectType::Rock;

		currentPoint += offset;
	}
}

void CaveMap::UpdateExtents(const CavePoint& inPoint)
{
	if (inPoint.Y > LowestDepth)
	{
		LowestDepth = inPoint.Y;
	}

	if (inPoint.X < FurthestLeft)
	{
		FurthestLeft = inPoint.X;
	}

	if (inPoint.X > FurthestRight)
	{
		FurthestRight = inPoint.X;
	}
}

bool CaveMap::DropSand(const CaveObject& inSandSpawner)
{
	CavePoint sandLocation = inSandSpawner.Location;

	bool canDrop = true;
	bool inAbyss = false;
	while (canDrop)
	{
		if (!HasFloor)
		{
			if (sandLocation.Y > LowestDepth)
			{
				inAbyss = true;
				break;
			}
		}

		CaveObject& downObject = GetCaveObject(sandLocation + DownOffset);

		if (downObject.Type == CaveObjectType::Air)
		{
			sandLocation = downObject.Location;
			continue;
		}
		
		CaveObject& downLeftObject = GetCaveObject(sandLocation + DownLeftOffset);

		if (downLeftObject.Type == CaveObjectType::Air)
		{
			sandLocation = downLeftObject.Location;
			continue;
		}

		CaveObject& downRrightObject = GetCaveObject(sandLocation + DownRightOffset);

		if (downRrightObject.Type == CaveObjectType::Air)
		{
			sandLocation = downRrightObject.Location;
			continue;
		}

		canDrop = false;
	}

	if (!inAbyss)
	{	
		CaveObject& dropLocation = GetCaveObject(sandLocation);
		dropLocation.Type = CaveObjectType::Sand;
		
	}

	if (sandLocation == inSandSpawner.Location)
	{
		inAbyss = true;
	}

	return !inAbyss;
}

CaveObject& CaveMap::GetCaveObject(const CavePoint& inPoint)
{
	CaveObject& object = CaveObjects[inPoint];
	object.Location = inPoint;

	if (HasFloor)
	{
		if (inPoint.Y >= LowestDepth + 2)
		{
			object.Type = CaveObjectType::Rock;
		}
	}
	return object;
}

const CaveObject& CaveMap::GetCaveObject(const CavePoint& inPoint) const
{
	return CaveObjects.at(inPoint);
}

CaveSandParser::CaveSandParser()
	:Map()
{
}

void CaveSandParser::OnStartParse()
{
	std::cout << "Figuring out where the sand will fall in the cave..." << std::endl;
}

void CaveSandParser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	Map.ParseRocks(inLine);
}

void CaveSandParser::OnEndParse()
{
	Map.SimulateSand();
	Map.DebugPrintMap();

	Map.ClearSand();
	Map.AddFloor();

	Map.SimulateSand();
	Map.DebugPrintMap();
}