#pragma once

#include "AdventParser.h"

namespace DirectoryCrawler {
	struct File {
		std::string Name;
		size_t Size;
	};

	
	struct Directory {
		std::string Name;
		std::vector<File> Files;
		std::vector<Directory> Directories;
		Directory* Parent;

		size_t GetSize() const;
	};

	typedef std::vector<Directory> DirectoryVector;

	typedef std::vector<DirectoryVector> DirectoryMap;

	class FileTree {
	public:
		FileTree();

		void AddFile(const std::string& inName, size_t inSize);
		void AddDirectory(const std::string& inName);

		void EnterDirectory(const std::string& inName);
		void UpDirectory();

		const Directory& GetRoot() const { return Root; }
	private:
		Directory Root;
		Directory* CurrentDirectory;
	};

	class DirectoryCrawlerParser : public AdventParser::ParserInterface {
	public:
		DirectoryCrawlerParser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

		size_t GetTotalUnder100k() const;
		size_t GetSmallestToDelete() const;
	private:
		enum class ParsingMode {
			ParsingCommands,
			ListingFiles,
		};

		FileTree CommunicatorFileTree;
		ParsingMode CurrentParsingMode;
	};
}