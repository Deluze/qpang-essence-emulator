#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/RoomWriter.h"
#include "qpang/room/Room.h"

class RoomList : public LobbyServerPacket
{
public:
	explicit RoomList(const std::vector<Room::Ptr>& rooms) : LobbyServerPacket(759)
	{
		const auto roomSize = static_cast<uint16_t>(rooms.size());

		writeShort(roomSize);
		writeShort(roomSize);
		writeShort(roomSize);

		for (const auto& room : rooms)
		{
			RoomWriter::write(this, room);
		}
	}
};
