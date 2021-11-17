#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/player/Player.h"
#include "qpang/player/friend/FriendManager.h"

#include "packets/lobby/outgoing/friend/SendCancelOutgoingFriend.h"
#include "packets/lobby/outgoing/friend/SendIncomingFriendCancelled.h"

class CancelOutgoingFriendRequestEvent final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto playerId = packet.readInt();

		if (const auto target = Game::instance()->getPlayer(playerId); target != nullptr)
		{
			const auto player = conn->getPlayer();

			player->getFriendManager()->removeOutgoing(playerId);
			target->getFriendManager()->removeIncoming(player->getId());

			target->send(SendIncomingFriendCancelled(player));
		}

		conn->send(SendCancelOutgoingFriend(playerId));
	}
};