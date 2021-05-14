#pragma once

#include "core/communication/packet/PacketEvent.h"

#include <vector>

#include "qpang/Game.h"
#include "qpang/player/Player.h"
#include "qpang/player/friend/FriendManager.h"

#include "packets/lobby/outgoing/friend/DenyIncomingFriend.h"
#include "packets/lobby/outgoing/friend/OutgoingFriendCancelled.h"

class AcceptIncomingFriendRequestEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		auto playerId = packet.readInt();

		auto player = conn->getPlayer();

		if (auto target = Game::instance()->getPlayer(playerId); target != nullptr)
		{
			player->getFriendManager()->acceptIncoming(target);
			target->getFriendManager()->onOutgoingAccepted(player);
		}
	}
};