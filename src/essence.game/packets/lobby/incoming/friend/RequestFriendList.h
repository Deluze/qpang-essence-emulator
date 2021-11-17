#pragma once

#include "core/communication/packet/PacketEvent.h"

#include <vector>

#include "qpang/player/Player.h"
#include "qpang/player/friend/FriendManager.h"

#include "packets/lobby/outgoing/friend/SendFriendList.h"

class RequestFriendList final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const std::vector<Friend> friends = conn->getPlayer()->getFriendManager()->list();
		conn->send(SendFriendList(friends));
	}
};