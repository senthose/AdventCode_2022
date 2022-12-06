#pragma once

#include "AdventParser.h"

namespace SignalDevice {
	class SignalDeviceParser : public AdventParser::ParserInterface {
	public:
		SignalDeviceParser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;
		
		size_t GetStartOfBuffer() const { return StartOfBuffer; }
		size_t GetStartOfRealBuffer() const { return StartOfRealBuffer; }
	private:
		size_t StartOfBuffer;
		size_t StartOfRealBuffer;
	};
}