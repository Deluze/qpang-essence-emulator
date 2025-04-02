#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/RoomWriter.h"
#include "qpang/room/Room.h"

class SendGameRoomList : public LobbyServerPacket
{
public:
	explicit SendGameRoomList(const std::vector<Room::Ptr>& rooms) : LobbyServerPacket(759)
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
