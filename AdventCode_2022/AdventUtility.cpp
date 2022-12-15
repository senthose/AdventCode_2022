#include "AdventUtility.h"

using namespace AdventUtility;

bool Point::operator==(const Point& inOther) const
{
	return (X == inOther.X) && (Y == inOther.Y);
}

bool Point::operator!=(const Point& inOther) const
{
	return (X != inOther.X) || (Y != inOther.Y);
}

Point Point::operator+(const Point& inRight) const
{
	return Point {X + inRight.X, Y + inRight.Y};
}

Point Point::operator-(const Point& inRight) const
{
	return Point {X - inRight.X, Y - inRight.Y};
}

Point& Point::operator+=(const Point& inOther)
{
	X += inOther.X;
	Y += inOther.Y;
	return *this;
}

int Point::GetManhattenDistance(const Point& inPointA, const Point& inPointB)
{
	const int xDistance = std::abs(inPointA.X - inPointB.X);
	const int yDistance = std::abs(inPointA.Y - inPointB.Y);

	return xDistance + yDistance;
}

bool ComparePoint::operator()(const Point& inLeft, const Point& inRight) const
{
	if (inLeft.Y == inRight.Y)
	{
		return inLeft.X < inRight.X;
	}
	else
	{
		return inLeft.Y < inRight.Y;
	}
}