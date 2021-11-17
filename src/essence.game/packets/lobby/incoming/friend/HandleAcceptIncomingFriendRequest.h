#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/player/Player.h"
#include "qpang/player/friend/FriendManager.h"

class HandleAcceptIncomingFriendRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto playerId = packet.readInt();

		const auto player = conn->getPlayer();

		if (const auto target = Game::instance()->getPlayer(playerId); target != nullptr)
		{
			player->getFriendManager()->acceptIncoming(target);
			target->getFriendManager()->onOutgoingAccepted(player);
		}
	}
};