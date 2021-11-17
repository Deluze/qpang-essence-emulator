#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/player/Player.h"
#include "qpang/player/friend/FriendManager.h"

class HandleSendFriendRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		packet.readEmpty(12);
		const auto nickname = packet.readStringTerminated(16);

		if (nickname.empty() || nickname.size() < 4 || nickname.size() > 16)
			return;

		const auto player = conn->getPlayer();

		if (nickname == player->getName())
			return; // lol

		if (!player->getFriendManager()->hasOutgoingSlotsLeft())
			return;

		const auto target = Game::instance()->getPlayer(nickname);

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