#include "Volcano.h"

#include <iostream>

using namespace Volcano;

ValveNetwork::ValveNetwork()
	:ValveRooms()
{
}

void ValveNetwork::ParseValveRoom(const std::string& inValveRoomLine)
{
	AdventParser::VectorString splitString = AdventParser::SplitString(inValveRoomLine, ' ');

	ValveRoom& currentRoom = GetValveRoom(splitString[1]);

	AdventParser::RemoveNonDigits(splitString[4]);

	size_t flowRate = std::stoull(splitString[4]);

	currentRoom.FlowRate = flowRate;

	size_t numConnections = splitString.size() - 9;

	for (size_t i = 0; i < numConnections; ++i)
	{
		std::string& connectionName = splitString[9 + i];
		AdventParser::RemoveCharFromString(connectionName, ',');

		ValveRoom& connectedRoom = GetValveRoom(connectionName);

		currentRoom.ConnectedRooms.push_back(&connectedRoom);
	}
}

ValveRoom& ValveNetwork::GetValveRoom(const std::string& inName)
{
	auto matchRoomName = [&inName](const ValveRoom& inRoom) -> bool
	{
		return inRoom.Name == inName;
	};

	ValveRoomList::iterator it = std::find_if(ValveRooms.begin(), ValveRooms.end(), matchRoomName);

	if (it == ValveRooms.end())
	{
		ValveRooms.emplace_back();

		ValveRoom& newRoom = ValveRooms.back();

		newRoom.Name = inName;

		return newRoom;
	}

	return *it;
}

const ValveRoom& ValveNetwork::GetValveRoom(const std::string& inName) const
{
	auto matchRoomName = [&inName](const ValveRoom& inRoom) -> bool
	{
		return inRoom.Name == inName;
	};

	ValveRoomList::const_iterator it = std::find_if(ValveRooms.begin(), ValveRooms.end(), matchRoomName);

	return *it;
}

size_t ValveNetwork::GetMaxPressureRelease()
{
	ResetValves();

	ValveRoom& startRoom = GetValveRoom("AA");


	ValveRoomPtrVector targets;
	for (ValveRoom& room : ValveRooms)
	{
		if (!room.Opened)
		{
			targets.push_back(&room);
		}
	}

	size_t timePassed = 0;
	size_t highestPressue = 0;

	ValveRoom* currentRoom = &startRoom;
	while (timePassed < 30)
	{
		currentRoom = MoveToBestValve(currentRoom, timePassed);

		if (AllValvesOpen())
		{
			break;
		}
	}

	highestPressue = CalculateTotalPressure();

	/*
	size_t numTargets = targets.size();
	ResetValves();
	timePassed = 0;
	MoveThroughRooms(currentRoom, targets[0], targets, timePassed, highestPressue);
	*/
	
	return highestPressue;
}

void ValveNetwork::ResetValve(ValveRoom& room)
{
	room.Opened = false;
	room.TimeOpened = 0;

	if (room.FlowRate == 0)
	{
		room.Opened = true;
	}
}

void ValveNetwork::ResetValves()
{
	for (ValveRoom& room : ValveRooms)
	{
		ResetValve(room);
	}
}

bool ValveNetwork::AllValvesOpen() const
{
	for (const ValveRoom& room : ValveRooms)
	{
		if (!room.Opened)
		{
			return false;
		}
	}

	return true;
}

void ValveNetwork::EnterRoom(ValveRoom* inRoom, size_t& outTimePassed)
{
	if (!inRoom->Opened)
	{
		++outTimePassed;
		inRoom->Opened = true;
		inRoom->TimeOpened = outTimePassed;
	}
}

ValveRoom* ValveNetwork::MoveToBestValve(ValveRoom* currentRoom, size_t& outTimePassed)
{
	if (!currentRoom->Opened)
	{
		EnterRoom(currentRoom, outTimePassed);

		if (outTimePassed >= 30)
		{
			return currentRoom;
		}
	}

	ValveRoom* nextRoom = GetNextRoom(currentRoom, outTimePassed);

	return nextRoom;
}

ValveRoom* ValveNetwork::GetNextRoom(ValveRoom* currentRoom, size_t& outTimePassed)
{
	ValveRoomPtrList targets;

	for (ValveRoom& room : ValveRooms)
	{
		if (!room.Opened)
		{
			targets.push_back(&room);
		}
	}

	struct TravelData {
		size_t FlowValue;
		ValveRoomPtrList Path;
	};

	typedef std::list<TravelData> TravelDataVector;

	TravelDataVector travelDatas;

	const TravelData* targetData = nullptr;

	size_t highestFlowValue = 0;
	size_t highestFlowDistance = 0;
	size_t lowestFlowDistance = std::numeric_limits<size_t>::max();
	for (ValveRoom* room : targets)
	{
		travelDatas.emplace_back();
		TravelData& data = travelDatas.back();

		size_t flowValue = room->FlowRate;
		data.FlowValue = flowValue;
		data.Path = GetShortestPath(currentRoom, room);

		
		/*
		if (highestFlowValue < flowValue)
		{
			highestFlowValue = flowValue;
			highestFlowDistance = data.Path.size();
			targetData = &data;
		}
		*/

		size_t pathDistance = data.Path.size();
		if (pathDistance < lowestFlowDistance)
		{
			lowestFlowDistance = pathDistance;
			targetData = &data;
		}
		
	}

	/*
	for (const TravelData& data : travelDatas)
	{
		if (data.FlowValue < highestFlowValue)
		{
			size_t pathSize = data.Path.size();
			if (pathSize < highestFlowDistance)
			{
				size_t newFlowValue = ((highestFlowDistance - pathSize) * data.FlowValue);
				if (newFlowValue > highestFlowValue)
				{
					highestFlowValue = newFlowValue;
					highestFlowDistance = pathSize;
					targetData = &data;
				}
			}
		}
	}
	*/
	

	if (targetData != nullptr)
	{
		for (ValveRoom* room : targetData->Path)
		{
			EnterRoom(currentRoom, outTimePassed);
			++outTimePassed;
		}

		return targetData->Path.back();
	}

	return nullptr;
}

void ValveNetwork::MoveThroughRooms(ValveRoom* currentRoom, ValveRoom* targetRoom, ValveRoomPtrVector& inTargetRooms, size_t& outTimePassed, size_t& outPressure)
{
	if (AllValvesOpen() || outTimePassed >= 30)
	{
		size_t pressure = CalculateTotalPressure();
		if (outPressure < pressure)
		{
			outPressure = pressure;
		}
		return;
	}

	EnterRoom(currentRoom, outTimePassed);

	size_t currentTime = outTimePassed;

	ValveRoom* lastRoom = currentRoom;
	for (size_t i = 0; i < inTargetRooms.size(); ++i)
	{
		ValveRoom* room = inTargetRooms[i];

		if (!room->Opened)
		{
			ValveRoomPtrList path = GetShortestPath(currentRoom, room);

			for (ValveRoom* room : path)
			{
				EnterRoom(currentRoom, outTimePassed);
				++outTimePassed;
			}

			MoveThroughRooms(lastRoom, room, inTargetRooms, outTimePassed, outPressure);

			lastRoom = room;

			for (size_t j = i; j < inTargetRooms.size(); ++j)
			{
				ResetValve(*inTargetRooms[j]);
			}
			
			outTimePassed = currentTime;
		}
	}
}

ValveRoomPtrList ValveNetwork::GetShortestPath(const ValveRoom* inFrom, const ValveRoom* inTo)
{
	struct PathNode {
		ValveRoom* Room;
		bool Visited = false;
		bool Target = false;
		size_t Distance = std::numeric_limits<size_t>::max();
		ValveRoom* Previous;
	};

	typedef std::vector<PathNode> PathVector;
	PathVector pathNodes;

	for (ValveRoom& room : ValveRooms)
	{
		pathNodes.push_back({&room});

		PathNode& node = pathNodes.back();

		if (node.Room == inFrom)
		{
			node.Visited = true;
			node.Distance = 0;
		}

		if (node.Room == inTo)
		{
			node.Target = true;
		}
	}

	auto GetPathNode = [&pathNodes](const ValveRoom* inRoom) -> PathNode&
	{
		auto matchRoom = [&inRoom](const PathNode& inNode) ->bool
		{
			return inNode.Room == inRoom;
		};
		
		PathVector::iterator it = std::find_if(pathNodes.begin(), pathNodes.end(), matchRoom);

		return *it;
	};


	auto CheckPath = [&pathNodes, &GetPathNode](PathNode* inNode) -> void 
	{
		for (ValveRoom* room : inNode->Room->ConnectedRooms)
		{
			PathNode& node = GetPathNode(room);

			if (!node.Visited)
			{
				size_t nodeCost = room->FlowRate > 0 ? 1 : 3;
				size_t distance = inNode->Distance + nodeCost;
				if (distance < node.Distance)
				{
					node.Distance = distance;
					node.Previous = inNode->Room;
				}
			}
		}
	};

	auto GetNextNode = [&pathNodes]() -> PathNode*
	{
		PathNode* closest = nullptr;
		size_t closestDistance = std::numeric_limits<size_t>::max();
		for (PathNode& node : pathNodes)
		{
			if (!node.Visited)
			{
				if (node.Distance < closestDistance)
				{
					closestDistance = node.Distance;
					closest = &node;
				}
			}
		}

		return closest;
	};

	PathNode& target = GetPathNode(inTo);

	PathNode* current = &GetPathNode(inFrom);

	while (!target.Visited)
	{
		CheckPath(current);

		current->Visited = true;

		current = GetNextNode();
	}

	ValveRoomPtrList path;

	current = &target;
	while (current != nullptr)
	{
		if (current->Room != inFrom)
		{
			path.push_front(current->Room);
		}

		if (current->Previous != nullptr)
		{
			current = &GetPathNode(current->Previous);
		}
		else
		{
			current = nullptr;
		}
	}

	return path;
}

size_t ValveNetwork::CalculateTotalPressure() const
{
	size_t totalPressure = 0;
	for (const ValveRoom& room : ValveRooms)
	{
		if (room.Opened && room.TimeOpened < 30)
		{
			totalPressure += room.FlowRate * (30 - room.TimeOpened);
		}
	}

	return totalPressure;
}

Parser::Parser()
	:Network()
{
}

void Parser::OnStartParse()
{
	std::cout << "Parsing the valve rooms..." << std::endl;
}

void Parser::ParseLine(const int inLineNumber, const std::string& inLine)
{
	Network.ParseValveRoom(inLine);
}

void Parser::OnEndParse()
{
	std::cout << "The max pressure that can be released is " << Network.GetMaxPressureRelease() << std::endl;
}
