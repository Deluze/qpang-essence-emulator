#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"

#include "qpang/player/Player.h"
#include "qpang/player/friend/FriendManager.h"

#include "packets/lobby/outgoing/friend/FriendList.h"

class RemoveFriendEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		auto playerId = packet.readInt();

		if (auto target = Game::instance()->getPlayer(playerId); target != nullptr)
		{
			auto player = conn->getPlayer();

			player->getFriendManager()->remove(target->getId());
			target->getFriendManager()->onRemoved(player->getId());
		}
	}
};