#pragma once

#include "AdventParser.h"

#include <limits>

namespace Hill {
	struct HillPoint {
		int X;
		int Y;

		HillPoint operator+(const HillPoint& inOtherPoint) const;
	};

	const HillPoint UpOffset {0, -1};
	const HillPoint DownOffset {0, 1};
	const HillPoint RightOffset{ 1, 0 };
	const HillPoint LeftOffset {-1, 0};

	struct HillNode {
		HillPoint Point;
		int DistanceFromStart = std::numeric_limits<int>::max();
		bool Visited = false;
		char Height;

		bool operator==(const HillNode& inOtherNode) const;
		bool operator!=(const HillNode& inOtherNode) const;
	};

	typedef std::vector<HillNode> HillVector;
	typedef std::vector<HillVector> HillVector2D;

	typedef std::vector<HillNode*> HillReferenceVector;
	
	typedef std::vector<std::string> StringVector;

	class HillMap {
	public:
		HillMap();

		void Initialize(const StringVector& inMapData);

		HillNode& GetNode(const HillPoint& inPoint);

		const HillNode& GetNode(const HillPoint& inPoint) const;

		size_t GetShortestPathLength() const;
	private:
		void MapPath();

		void DebugDrawMap() const;

		HillVector2D Map;
		HillPoint StartPoint;
		HillPoint EndPoint;
	};

	class HillParser : public AdventParser::ParserInterface {
	public:
		HillParser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

	private:
		HillMap Map;
		StringVector MapData;
	};
}