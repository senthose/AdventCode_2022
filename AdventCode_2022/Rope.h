#pragma once

#include "AdventParser.h"
#include <forward_list>
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

	struct RopeNode;

	typedef std::array<RopeNode*, static_cast<size_t>(MoveDirection::Size)> RopeDirections;

	struct RopeNode {
		RopeNode();

		bool TailEntered = false;

		RopeDirections Directions;

		RopeNode* GetNode(MoveDirection inDirection);
		const RopeNode* GetNode(MoveDirection inDirection) const;

		void SetNode(MoveDirection inDirection, RopeNode* inNode);
	};

	class RopeMap {
	public:
		RopeMap();

		void MoveHead(MoveDirection inDirection);

		size_t GetNumTailSpaces() const;

	private:
		RopeNode* MakeNode();
		RopeNode* CreateNode(MoveDirection fromDirection, RopeNode* inAdjacentNode);

		RopeNode* GetNodeInDirection(MoveDirection inDirection, RopeNode* inCurrentNode);

		std::forward_list<RopeNode> Nodes;
		RopeNode* HeadPosition;
		RopeNode* TailPosition;
		RopeNode* StartPosition;
	};

	class RopeParser : public AdventParser::ParserInterface {
	public:
		RopeParser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

		size_t GetNumTailSpaces() const;
	private:
		RopeMap Map;
	};
}