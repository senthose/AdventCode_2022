#include "Beacons.h"

#include <iostream>

using namespace Beacon;
using namespace AdventUtility;

const char Object::ToChar() const
{
	switch (Type)
	{
	case Beacon::ObjectType::Unknown:
		return '?';
	case Beacon::ObjectType::Sensor:
		return 'S';
	case Beacon::ObjectType::Beacon:
		return 'B';
	case Beacon::ObjectType::SensedArea:
		return '#';
	default:
		return '?';
	}
}

BeaconMap::BeaconMap()
	:Map()
	,LeftSensorExtent(MAX_INT)
	,RightSensorExtent(MIN_INT)
{
}

void BeaconMap::ParseSensor(const std::string& inSensorLine)
{
	AdventParser::VectorString splitString = AdventParser::SplitString(inSensorLine, ':');

	AdventParser::VectorString sensorSplit = AdventParser::SplitString(splitString[0], ',');
	AdventParser::VectorString beaconSplit = AdventParser::SplitString(splitString[1], ',');

	AdventParser::RemoveNonDigits(sensorSplit[0]);
	AdventParser::RemoveNonDigits(sensorSplit[1]);

	AdventParser::RemoveNonDigits(beaconSplit[0]);
	AdventParser::RemoveNonDigits(beaconSplit[1]);

	const int sensorX = std::stoi(sensorSplit[0]);
	const int sensorY = std::stoi(sensorSplit[1]);

	const Point sensorPoint {sensorX, sensorY};

	const int beaconX = std::stoi(beaconSplit[0]);
	const int beaconY = std::stoi(beaconSplit[1]);

	const Point beaconPoint {beaconX, beaconY};

	Object& sensor = Map.Get(sensorPoint);
	sensor.Type = ObjectType::Sensor;
	sensor.Distance = Point::GetManhattenDistance(sensorPoint, beaconPoint);

	Object& beacon = Map.Get(beaconPoint);
	beacon.Type = ObjectType::Beacon;

	UpdateSensorExtents(sensor, sensorPoint);

	Sensors.push_back(std::make_pair(sensorPoint, sensor));
}

size_t BeaconMap::GetNumSpacesSensedAtLine(int inLineIndex) const
{
	size_t numScannedPoints = 0;

	Point scanningPoint;
	scanningPoint.Y = inLineIndex;
	for (scanningPoint.X = LeftSensorExtent; scanningPoint.X <= RightSensorExtent; ++scanningPoint.X)
	{
		if (!Map.Exists(scanningPoint))
		{
			if (InRangeOfBeacon(scanningPoint))
			{
				++numScannedPoints;
			}
		}
	}

	return numScannedPoints;
}

int BeaconMap::GetDistressBeaconFrequency() const
{
	typedef std::vector<Point> PointVector;

	const int searchStartX = 0;
	const int searchEndX = 4000000;
	const int searchStartY = 0;
	const int searchEndY = 4000000;

	auto InSearchArea = [searchStartX, searchEndX, searchStartY, searchEndY](const Point& inPoint) -> bool
	{
		return (inPoint.X >= searchStartX && inPoint.X <= searchEndX && inPoint.Y >= searchStartY && inPoint.Y <= searchEndY);
	};

	auto GetPointsAround = [InSearchArea](const ObjectPair& inSensorPair, PointVector& outVector) -> void
	{
		outVector.clear();

		const Point& sensorPoint = inSensorPair.first;
		const int distance = inSensorPair.second.Distance + 1;

		for (int i = 0; i <= distance; ++i)
		{
			const Point topRightOffset {distance - i, i};
			const Point topLeftOffset {-(distance - i), i};
			const Point bottomRightOffset {distance - i, -i};
			const Point bottomLeftOffset {-(distance - i), -i};

			const Point topRight = sensorPoint + topRightOffset;
			const Point topLeft = sensorPoint + topLeftOffset;

			if (InSearchArea(topRight))
			{
				outVector.push_back(topRight);
			}

			if (topRight != topLeft)
			{
				if (InSearchArea(topLeft))
				{
					outVector.push_back(topLeft);
				}
			}

			if (i > 0)
			{
				const Point bottomRight = sensorPoint + bottomRightOffset;
				const Point bottomLeft = sensorPoint + bottomLeftOffset;

				if (InSearchArea(bottomRight))
				{
					outVector.push_back(bottomRight);
				}

				if (bottomRight != bottomLeft)
				{
					if (InSearchArea(bottomLeft))
					{
						outVector.push_back(bottomLeft);
					}
				}
			}
		}
	};

	Point scanningPoint;

	PointVector points;
	bool foundPoint = false;

	for (const ObjectPair& objectPair : Sensors)
	{
		GetPointsAround(objectPair, points);

		for (const Point& point : points)
		{
			if (!InRangeOfBeacon(point))
			{
				scanningPoint = point;
				foundPoint = true;
				break;
			}
		}

		if (foundPoint)
		{
			break;
		}
	}
	
	return (scanningPoint.X * 4000000) + scanningPoint.Y;
}

void BeaconMap::DebugPrintMap() const
{
	auto printFunction = [](const Point& inPoint, const Object& inObject)
	{
		std::cout << inObject.ToChar();
	};

	Map.DebugPrint(printFunction, '.');
}

void BeaconMap::UpdateSensorExtents(const Object& inSensor, const AdventUtility::Point& inPoint)
{
	const int inLeftSensorExtent = inPoint.X - inSensor.Distance;
	const int inRightSensorExtent = inPoint.X + inSensor.Distance;

	if (LeftSensorExtent > inLeftSensorExtent)
	{
		LeftSensorExtent = inLeftSensorExtent;
	}

	if (RightSensorExtent < inRightSensorExtent)
	{
		RightSensorExtent = inRightSensorExtent;
	}
}

bool BeaconMap::InRangeOfBeacon(const AdventUtility::Point& inPoint) const
{
	for (const ObjectPair& objectPair : Sensors)
	{
		const int pointDistance = Point::GetManhattenDistance(inPoint, objectPair.first);

		if (pointDistance <= objectPair.second.Distance)
		{
			return true;
		}
	}

	return false;
}

Parser::Parser()
	:Map()
{
}

void Parser::OnStartParse()
{
	std::cout << "Parsing the sensor map..." << std::endl;
}

void Parser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	Map.ParseSensor(inLine);
}

void Parser::OnEndParse()
{
	std::cout << "The number of spaces that can not contain beacons is " << Map.GetNumSpacesSensedAtLine(2000000) << std::endl;
	std::cout << "The tuning frequency of the distress beacon is " << Map.GetDistressBeaconFrequency() << std::endl;
}