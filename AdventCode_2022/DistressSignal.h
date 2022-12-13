#pragma once

#include "AdventParser.h"

namespace Distress {
	enum class OrderType {
		InOrder,
		OutOfOrder,
		Inconclusive,
	};

	struct Data {
		int Value = -1;
		std::vector<Data> Children;
		Data* Parent;

		bool IsValue() const;

		static bool AreEqual(const Data& A, const Data& B);
	};

	typedef std::vector<Data> DataVector;

	struct Packet {
		Data DataList;
		
		void Parse(const std::string& inPacketString);
		bool HasParsed() const { return Parsed; }
	private:
		bool Parsed = false;
	};
	
	struct PacketPair {
		Packet Left;
		Packet Right;

		static OrderType CompareData(const Data& leftData, const Data& rightData);

		bool IsInCorrectOrder() const;		
	};

	typedef std::vector<PacketPair> PacketPairVector;

	class DistressSignalParser : public AdventParser::ParserInterface {
	public:
		DistressSignalParser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

	private:
		void CheckOrder() const;
		void FindDecoderKey() const;

		PacketPairVector PacketPairs;
	};
}