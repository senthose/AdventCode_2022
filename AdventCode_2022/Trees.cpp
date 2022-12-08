#include "Trees.h"

using namespace Trees;

bool IsTreeVisible(const Tree* inTree)
{
	size_t height = inTree->Height;

	bool visibleFromNorth = true;
	bool visibleFromSouth = true;
	bool visibleFromEast = true;
	bool visibleFromWest = true;

	const Tree* currentTree = inTree;
	while (currentTree->North != nullptr)
	{
		if (currentTree->North->Height >= height)
		{
			visibleFromNorth = false;
			break;
		}
		currentTree = currentTree->North;
	}

	currentTree = inTree;
	while (currentTree->South != nullptr)
	{
		if (currentTree->South->Height >= height)
		{
			visibleFromSouth = false;
			break;
		}
		currentTree = currentTree->South;
	}

	currentTree = inTree;
	while (currentTree->East != nullptr)
	{
		if (currentTree->East->Height >= height)
		{
			visibleFromEast = false;
			break;
		}
		currentTree = currentTree->East;
	}

	currentTree = inTree;
	while (currentTree->West != nullptr)
	{
		if (currentTree->West->Height >= height)
		{
			visibleFromWest = false;
			break;
		}
		currentTree = currentTree->West;
	}

	return visibleFromNorth || visibleFromSouth || visibleFromEast || visibleFromWest;
}

size_t GetTreeScenicScor(const Tree* inTree)
{
	size_t height = inTree->Height;
	size_t northDistance = 0;
	size_t southDistance = 0;
	size_t eastDistance = 0;
	size_t westDistance = 0;

	const Tree* currentTree = inTree;
	while (currentTree->North != nullptr)
	{
		++northDistance;
		if (currentTree->North->Height >= height)
		{
			break;
		}
		currentTree = currentTree->North;
	}

	currentTree = inTree;
	while (currentTree->South != nullptr)
	{
		++southDistance;
		if (currentTree->South->Height >= height)
		{
			break;
		}
		currentTree = currentTree->South;
	}

	currentTree = inTree;
	while (currentTree->East != nullptr)
	{
		++eastDistance;
		if (currentTree->East->Height >= height)
		{
			break;
		}
		currentTree = currentTree->East;
	}

	currentTree = inTree;
	while (currentTree->West != nullptr)
	{
		++westDistance;
		if (currentTree->West->Height >= height)
		{
			break;
		}
		currentTree = currentTree->West;
	}

	return northDistance * southDistance * eastDistance * westDistance;
}

TreeGrid::TreeGrid()
	:Trees()
{
}

void TreeGrid::SetSize(size_t inWidth, size_t inHeight)
{
	Trees.clear();

	Trees.resize(inWidth);
	
	for (TreeVector& treeColumn : Trees)
	{
		treeColumn.resize(inHeight);
	}

	for (size_t x = 0; x < inWidth; ++x)
	{
		for (size_t y = 0; y < inHeight; ++y)
		{
			Tree& tree = Trees[x][y];

			if (x > 0)
			{
				tree.West = &Trees[x - 1][y];
			}

			if (x < (inWidth - 1))
			{
				tree.East = &Trees[x + 1][y];
			}

			if (y > 0)
			{
				tree.North = &Trees[x][y - 1];
			}

			if (y < (inHeight - 1))
			{
				tree.South = &Trees[x][y + 1];
			}
		}
	}
}

void TreeGrid::AddTree(size_t xIndex, size_t yIndex, size_t inHeight)
{	
	Tree& tree = Trees[xIndex][yIndex];
	tree.Height = inHeight;
}

TreeParser::TreeParser()
	:Trees()
{
}

void TreeParser::OnStartParse()
{
}

void TreeParser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	const size_t sideLength = inLine.size();
	if (inLineNumber == 1)
	{
		Trees.SetSize(sideLength, sideLength);
	}

	size_t yIndex = inLineNumber - 1;
	for (size_t i = 0; i < sideLength; ++i)
	{
		const char heightChar = inLine[i];

		size_t treeHeight = heightChar - '0';

		Trees.AddTree(i, yIndex, treeHeight);
	}
}

void TreeParser::OnEndParse()
{
}

size_t TreeParser::GetNumTreesVisible() const
{
	const TreeVector2D& trees = Trees.GetTrees();

	size_t numVisible = 0;
	for (const TreeVector& treeColumn : trees)
	{
		for (const Tree& tree : treeColumn)
		{
			if (IsTreeVisible(&tree))
			{
				++numVisible;
			}
		}
	}
	
	return numVisible;
}

size_t TreeParser::GetHighestScenicScore() const
{
	const TreeVector2D& trees = Trees.GetTrees();

	size_t highestScore = 0;
	for (const TreeVector& treeColumn : trees)
	{
		for (const Tree& tree : treeColumn)
		{
			size_t treeScore = GetTreeScenicScor(&tree);
			if (treeScore > highestScore)
			{
				highestScore = treeScore;
			}
		}
	}

	return highestScore;
}