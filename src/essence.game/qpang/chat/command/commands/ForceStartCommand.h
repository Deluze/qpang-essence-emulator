#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

#include "qpang/room/session/RoomSession.h"
#include "qpang/room/RoomPlayer.h"

class ForceStartCommand : public Command
{
public:
	ForceStartCommand() : Command(4)
	{

	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
			return player->broadcast(u"You need to be in a room in order to use this command!");

		auto roomSession = roomPlayer->getRoom()->getRoomSession();

		if (roomSession != nullptr)
			return player->broadcast(u"Can't force start a room while playing!");

		roomPlayer->getRoom()->start();
	}
};