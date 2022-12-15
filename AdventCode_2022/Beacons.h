#pragma once

#include "AdventParser.h"
#include "AdventUtility.h"

namespace Beacon {
	enum class ObjectType {
		Unknown,
		Sensor,
		Beacon,
		SensedArea,
	};

	struct Object {
		ObjectType Type = ObjectType::Unknown;
		int Distance = 0;

		const char ToChar() const;
	};

	typedef AdventUtility::DynamicMap<Object> ObjectMap;

	class BeaconMap {
	public:
		typedef ObjectMap::MapType::value_type ObjectPair;
		typedef std::vector<ObjectPair> ObjectPairVector;

		BeaconMap();

		void ParseSensor(const std::string& inSensorLine);

		size_t GetNumSpacesSensedAtLine(int inLineIndex) const;
		int GetDistressBeaconFrequency() const;

		// This map is too big. 
		void DebugPrintMap() const;
	private:
		void UpdateSensorExtents(const Object& inSensor, const AdventUtility::Point& inPoint);
		bool InRangeOfBeacon(const AdventUtility::Point& inPoint) const;

		ObjectMap Map;
		ObjectPairVector Sensors;
		int LeftSensorExtent;
		int RightSensorExtent;
	};

	class Parser : public AdventParser::ParserInterface {
	public:
		Parser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

	private:
		BeaconMap Map;
	};
}