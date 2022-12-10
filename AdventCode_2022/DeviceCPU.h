#pragma once

#include "AdventParser.h"
#include <array>


namespace DeviceCPU {
	enum class Instruction {
		Noop,
		Addx,
		Invalid,
	};

	const int ScreenWidth = 40;
	const int ScreenHeight = 6;

	typedef std::array<char, ScreenWidth> CharArray;
	typedef std::array<CharArray, ScreenHeight> CharArray2D;

	class CPU {
	public:
		CPU();

		void RunAdd(int inValue);
		void RunNoop();

		int GetSignalSum() const;
		std::string GetDeviceScreen() const;

	private:
		void TickCycle();
		void UpdateSpritePosition();
		void DrawPixel();

		CharArray2D DeviceScreen;
		CharArray SpriteBuffer;

		std::vector<int> SignalPoints;
		int CycleCounter;
		int RegisterX;
	};

	class Parser : public AdventParser::ParserInterface {
	public:
		Parser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;


		int GetSignalSum() const;
		std::string GetDeviceScreen() const;
	private:
		CPU DeviceCPU;
	};
}