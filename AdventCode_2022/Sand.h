#pragma once

#include "AdventParser.h"
#include <map>

namespace Sand {
	enum class CaveObjectType {
		Air,
		Rock,
		Sand,
		SandSpawner,
	};

	struct CavePoint {
		int X = 0;
		int Y = 0;

		static CavePoint Parse(const std::string& inPointString);

		bool operator<(const CavePoint& inOther) const;

		bool operator==(const CavePoint& inOther) const;
		bool operator!=(const CavePoint& inOther) const;
		CavePoint operator-(const CavePoint& inRight) const;
		CavePoint operator+(const CavePoint& inRight) const;

		CavePoint& operator+=(const CavePoint& inOther);
	};

	const CavePoint DownOffset {0, 1};
	const CavePoint DownLeftOffset {-1, 1};
	const CavePoint DownRightOffset {1, 1};

	const CavePoint UpOffset {0, -1};
	const CavePoint RightOffset {1, 0};
	const CavePoint LeftOffset {-1, 0};

	const CavePoint NoOffset {0, 0};

	struct CaveObject {
		CavePoint Location;
		CaveObjectType Type = CaveObjectType::Air;

		const char ToChar() const;
	};

	typedef std::map<CavePoint, CaveObject> CaveMapTree;

	class CaveMap {
	public:
		CaveMap();

		void ParseRocks(const std::string& inWallString);
		void SimulateSand();
		void ClearSand();

		void AddFloor();

		void DebugPrintMap() const;
	private:
		void AddRocks(const CavePoint& inFrom, const CavePoint& inTo);
		void UpdateExtents(const CavePoint& inPoint);

		bool DropSand(const CaveObject& inSandSpawner);

		CaveObject& GetCaveObject(const CavePoint& inPoint);
		const CaveObject& GetCaveObject(const CavePoint& inPoint) const;

		CaveMapTree CaveObjects;
		int LowestDepth;
		int FurthestRight;
		int FurthestLeft;
		bool HasFloor;
	};

	class CaveSandParser : public AdventParser::ParserInterface {
	public:
		CaveSandParser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

	private:
		CaveMap Map;
	};
}