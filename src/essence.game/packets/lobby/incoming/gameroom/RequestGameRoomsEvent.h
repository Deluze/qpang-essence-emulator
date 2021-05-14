#pragma once

#include <string>
#include <cstdint>
#include <vector>

#include "core/communication/packet/PacketEvent.h"
#include "qpang/Game.h"
#include "qpang/room/RoomManager.h"

#include "packets/lobby/outgoing/gameroom/RoomList.h"

class RequestGameRoomsEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		auto rooms = Game::instance()->getRoomManager()->list();

		conn->send(RoomList(rooms));
	}
};