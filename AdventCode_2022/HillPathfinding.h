#pragma once

#include "AdventParser.h"

#include <limits>
#include <list>

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

	struct HillNode;

	struct HillNode {
		HillPoint Point;
		const HillNode* Previous;
		int DistanceFromStart = std::numeric_limits<int>::max();
		bool Visited = false;
		char Height;

		bool operator==(const HillNode& inOtherNode) const;
		bool operator!=(const HillNode& inOtherNode) const;
	};

	typedef std::vector<HillNode> HillVector;
	typedef std::vector<HillVector> HillVector2D;

	typedef std::vector<HillNode*> HillReferenceList;
	typedef std::list<const HillNode*> HillNodeList;
	
	typedef std::vector<std::string> StringVector;

	class HillMap {
	public:
		HillMap();

		void Initialize(const StringVector& inMapData);

		HillNode& GetNode(const HillPoint& inPoint);

		const HillNode& GetNode(const HillPoint& inPoint) const;

		size_t GetShortestPathLength() const;
		size_t GetShortestScenicPathLength() const;
	private:
		void Reset();

		bool FoundEnd() const;
		void CheckNodes(HillNode* inNode, int inHeight, int inWidth, HillReferenceList& outUnvisitedNodes);
		void MapPath(HillNode* inStart);
		void MapPaths();

		void DebugDrawMap() const;

		HillVector2D Map;
		HillPoint StartPoint;
		HillPoint EndPoint;
		size_t ShortestPath;
		size_t ShortestScenicPath;
	};

	class HillParser : public AdventParser::ParserInterface {
	public:
		HillParser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

		size_t GetShortestPath() const;
		size_t GetShortestScenicPath() const;
	private:
		HillMap Map;
		StringVector MapData;
	};
}