#pragma once

#include "core/communication/packet/PacketEvent.h"

#include <vector>

#include "qpang/player/Player.h"
#include "qpang/player/friend/FriendManager.h"

#include "packets/lobby/outgoing/friend/FriendList.h"

class RequestFriendList : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		std::vector<Friend> friends = conn->getPlayer()->getFriendManager()->list();
		conn->send(FriendList(friends));
	}
};