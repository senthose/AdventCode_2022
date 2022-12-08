#include "DirectoryCrawler.h"

using namespace DirectoryCrawler;

FileTree::FileTree()
	:Root()
	,CurrentDirectory(nullptr)
{
	CurrentDirectory = &Root;
	Root.Name = "/";
}

void FileTree::AddFile(const std::string& inName, size_t inSize)
{
	if (CurrentDirectory)
	{
		CurrentDirectory->Files.push_back(File{ inName, inSize });
	}
}

void FileTree::AddDirectory(const std::string& inName)
{
	if (CurrentDirectory)
	{
		Directory newDirectory;
		newDirectory.Name = inName;
		newDirectory.Parent = CurrentDirectory;

		CurrentDirectory->Directories.push_back(newDirectory);
	}
}

void FileTree::EnterDirectory(const std::string& inName)
{
	if (CurrentDirectory)
	{
		DirectoryVector& currentDirectoryVector = CurrentDirectory->Directories;

		for (Directory& possibleDirectory : currentDirectoryVector)
		{
			if (possibleDirectory.Name == inName)
			{
				CurrentDirectory = &possibleDirectory;
				break;
			}
		}
	}
}

void FileTree::UpDirectory()
{
	if (CurrentDirectory->Parent != nullptr)
	{
		CurrentDirectory = CurrentDirectory->Parent;
	}
}

DirectoryCrawlerParser::DirectoryCrawlerParser()
	:CommunicatorFileTree()
	,CurrentParsingMode(ParsingMode::ParsingCommands)
{
}

void DirectoryCrawlerParser::OnStartParse()
{
	CurrentParsingMode = ParsingMode::ParsingCommands;
}

void DirectoryCrawlerParser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	if (!inLine.empty())
	{
		const AdventParser::VectorString splitString = AdventParser::SplitString(inLine, ' ');

		if (splitString.size() > 0)
		{
			if (splitString[0] == "$")
			{
				CurrentParsingMode = ParsingMode::ParsingCommands;

				if (splitString[1] == "cd")
				{
					if (splitString[2] == "..")
					{
						CommunicatorFileTree.UpDirectory();
					}
					else
					{
						CommunicatorFileTree.EnterDirectory(splitString[2]);
					}
				}

				if (splitString[1] == "ls")
				{
					CurrentParsingMode = ParsingMode::ListingFiles;
				}
			}
			else
			{
				switch (CurrentParsingMode)
				{
				case ParsingMode::ListingFiles:
					if (splitString[0] == "dir")
					{
						CommunicatorFileTree.AddDirectory(splitString[1]);
					}
					else
					{
						CommunicatorFileTree.AddFile(splitString[1], std::stoull(splitString[0]));
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

void DirectoryCrawlerParser::OnEndParse()
{
}

size_t FindSizes(const Directory& inDirectory, size_t& outTotal)
{
	size_t totalSize = 0;
	for (const File& file : inDirectory.Files)
	{
		totalSize += file.Size;
	}
	
	for (const Directory& directory : inDirectory.Directories)
	{
		totalSize += FindSizes(directory, outTotal);
	}

	if (totalSize <= 100000)
	{
		outTotal += totalSize;
	}

	return totalSize;
}

size_t FindMinMax(const Directory& inDirectory, size_t& outCurrentMax, size_t min)
{
	size_t totalSize = 0;
	for (const File& file : inDirectory.Files)
	{
		totalSize += file.Size;
	}

	for (const Directory& directory : inDirectory.Directories)
	{
		totalSize += FindMinMax(directory, outCurrentMax, min);
	}

	if (totalSize >= min && totalSize < outCurrentMax)
	{
		outCurrentMax = totalSize;
	}

	return totalSize;
}

size_t DirectoryCrawlerParser::GetTotalUnder100k() const
{
	const Directory& Root = CommunicatorFileTree.GetRoot();

	size_t total = 0;

	FindSizes(Root, total);

	return total;
}

size_t DirectoryCrawlerParser::GetSmallestToDelete() const
{
	const Directory& Root = CommunicatorFileTree.GetRoot();

	size_t total = 0;

	size_t rootSize = FindSizes(Root, total);

	size_t unusedSpace = 70000000 - rootSize;

	if (unusedSpace < 30000000)
	{
		size_t minDeleteSize = 30000000 - unusedSpace;

		size_t minMax = 70000000;
		FindMinMax(Root, minMax, minDeleteSize);

		return minMax;
	}
	
	return rootSize;
}