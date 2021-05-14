#pragma once

#include "core/communication/packet/PacketEvent.h"

#include <vector>

#include "qpang/Game.h"
#include "qpang/player/Player.h"
#include "qpang/player/friend/FriendManager.h"

#include "packets/lobby/outgoing/friend/FriendList.h"

class SendFriendRequestEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		packet.readEmpty(12);
		auto nickname = packet.readStringTerminated(16);

		if (nickname.empty() || nickname.size() < 4 || nickname.size() > 16)
			return;

		auto player = conn->getPlayer();

		if (nickname == player->getName())
			return; // lol

		if (!player->getFriendManager()->hasOutgoingSlotsLeft())
			return;

		auto target = Game::instance()->getPlayer(nickname);

		if (target == nullptr)
			return;

		if (!target->getFriendManager()->hasIncomingSlotsLeft())
			return;

		if (player->getFriendManager()->contains(target->getId()) || target->getFriendManager()->contains(player->getId()))
			return;

		player->getFriendManager()->addOutgoingFriend(target);
		target->getFriendManager()->addIncomingFriend(player);
	}
};