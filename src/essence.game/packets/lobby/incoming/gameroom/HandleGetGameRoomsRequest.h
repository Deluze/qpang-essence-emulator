#pragma once

#include <vector>

#include "core/communication/packet/PacketEvent.h"
#include "qpang/Game.h"
#include "qpang/room/RoomManager.h"

#include "packets/lobby/outgoing/gameroom/SendGameRoomList.h"

class HandleGetGameRoomsRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto rooms = Game::instance()->getRoomManager()->list();

		conn->send(SendGameRoomList(rooms));
	}
};