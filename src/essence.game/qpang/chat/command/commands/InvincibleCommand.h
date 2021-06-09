#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

#include "qpang/room/session/RoomSession.h"
#include "qpang/room/RoomPlayer.h"

class InvincibleCommand : public Command
{
public:
	InvincibleCommand() : Command(3)
	{

	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr) {
			player->broadcast(u"You need to be in a room in order to use this command.");

			return;
		}

		auto roomSession = roomPlayer->getRoom()->getRoomSession();

		if (roomSession == nullptr) {
			player->broadcast(u"This room is not in a game.");

			return;
		}

		if (!roomPlayer->isPlaying()) {
			player->broadcast(u"You are not in a game.");

			return;
		}

		roomPlayer->getRoomSessionPlayer()->makePermanentlyInvincible();

		player->broadcast(u"You are now invincible.");
	}
};