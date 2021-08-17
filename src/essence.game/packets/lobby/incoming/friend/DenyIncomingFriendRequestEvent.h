#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/player/Player.h"
#include "qpang/player/friend/FriendManager.h"

#include "packets/lobby/outgoing/friend/DenyIncomingFriend.h"
#include "packets/lobby/outgoing/friend/OutgoingFriendCancelled.h"

class DenyIncomingFriendRequestEvent final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto playerId = packet.readInt();

		const auto player = conn->getPlayer();

		if (const auto target = Game::instance()->getPlayer(playerId); target != nullptr)
		{
			player->getFriendManager()->removeIncoming(playerId);
			target->getFriendManager()->removeOutgoing(player->getId());

			target->send(OutgoingFriendCancelled(player));
		}

		conn->send(DenyIncomingFriend(playerId));
	}
};