#pragma once

#include "AdventParser.h"

#include <list>

namespace Volcano {
	struct ValveRoom {
		std::string Name;
		size_t FlowRate = 0;
		bool Opened = false;
		size_t TimeOpened = 0;
		std::list<ValveRoom*> ConnectedRooms;
	};

	typedef std::list<ValveRoom*> ValveRoomPtrList;
	typedef std::list<ValveRoom> ValveRoomList;

	class ValveNetwork {
	public:
		ValveNetwork();

		void ParseValveRoom(const std::string& inValveRoomLine);
		ValveRoom& GetValveRoom(const std::string& inName);
		const ValveRoom& GetValveRoom(const std::string& inName) const;

		size_t GetMaxPressureRelease();
	private:
		void ResetValves();

		bool AllValvesOpen() const;
		void EnterRoom(ValveRoom* inRoom, size_t& outTimePassed);
		ValveRoom* MoveToBestValve(ValveRoom* currentRoom, size_t& outTimePassed);
		ValveRoom* GetNextRoom(ValveRoom* currentRoom, size_t& outTimePassed);

		ValveRoomPtrList GetShortestPath(const ValveRoom* inFrom, const ValveRoom* inTo);

		size_t CalculateTotalPressure() const;

		ValveRoomList ValveRooms;
	};

	class Parser :public AdventParser::ParserInterface {
	public:
		Parser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

	private:
		ValveNetwork Network;
	};
}