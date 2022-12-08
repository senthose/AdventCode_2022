#pragma once

#include "AdventParser.h"

namespace Trees {
	struct Tree {
		size_t Height = 0;
		Tree* North = nullptr;
		Tree* South = nullptr;
		Tree* East = nullptr;
		Tree* West = nullptr;
	};

	typedef std::vector<Tree> TreeVector;
	typedef std::vector<TreeVector> TreeVector2D;

	class TreeGrid {
	public:
		TreeGrid();

		void SetSize(size_t inWidth, size_t inHeight);
		void AddTree(size_t xIndex, size_t yIndex, size_t inHeight);

		const TreeVector2D& GetTrees() const { return Trees; }
	private:
		TreeVector2D Trees;
	};
	

	class TreeParser : public AdventParser::ParserInterface {
	public:
		TreeParser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

		size_t GetNumTreesVisible() const;
		size_t GetHighestScenicScore() const;
	private:
		TreeGrid Trees;
	};
}