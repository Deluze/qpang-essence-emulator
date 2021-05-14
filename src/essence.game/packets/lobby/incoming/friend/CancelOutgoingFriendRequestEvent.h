#pragma once

#include "core/communication/packet/PacketEvent.h"

#include <vector>

#include "qpang/Game.h"
#include "qpang/player/Player.h"
#include "qpang/player/friend/FriendManager.h"

#include "packets/lobby/outgoing/friend/FriendList.h"
#include "packets/lobby/outgoing/friend/CancelOutgoingFriend.h"
#include "packets/lobby/outgoing/friend/IncomingFriendCancelled.h"

class CancelOutgoingFriendRequestEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		auto playerId = packet.readInt();

		if (auto target = Game::instance()->getPlayer(playerId); target != nullptr)
		{
			auto player = conn->getPlayer();

			player->getFriendManager()->removeOutgoing(playerId);
			target->getFriendManager()->removeIncoming(player->getId());

			target->send(IncomingFriendCancelled(player));
		}

		conn->send(CancelOutgoingFriend(playerId));
	}
};