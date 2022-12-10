#pragma once

#include "AdventParser.h"
#include <forward_list>
#include <bitset>
#include <array>

namespace Rope {
	enum class MoveDirection {
		North,
		South,
		East,
		West,
		NorthEast,
		NorthWest,
		SouthEast,
		SouthWest,
		Size,
	};

	const int NumRopeSegments = 10;

	struct RopeNode {
		std::bitset<NumRopeSegments> VisitBits;

		void SetVisited(size_t inRopeIndex);
		bool WasVisited(size_t inRopeIndex) const;
	};

	struct RopePoint {
		int X;
		int Y;
	};
	
	typedef std::vector<RopeNode> RopeVector;
	typedef std::vector<RopeVector> RopeVector2D;

	class RopeMap {
	public:
		RopeMap();

		void MoveHead(MoveDirection inDirection);

		size_t GetNumTailSpaces(size_t inTailIndex) const;
	private:
		bool IsValidCoord(int x, int y) const;
		bool IsValidCoord(const RopePoint& inPoint) const;
		void SetGridSize(int newHalfWidth);

		RopeVector2D RopeGrid;
		std::array<RopePoint, NumRopeSegments> RopePoints;
		int HalfWidth;
	};

	class RopeParser : public AdventParser::ParserInterface {
	public:
		RopeParser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

		size_t GetNumTailSpaces(size_t inTailIndex) const;
	private:
		RopeMap Map;
	};
}