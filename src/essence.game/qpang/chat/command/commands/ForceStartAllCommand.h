#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

#include "qpang/room/Room.h"
#include "qpang/room/RoomPlayer.h"
#include "qpang/room/session/RoomSession.h"

class ForceStartAllCommand : public Command
{
public:
	ForceStartAllCommand() : Command(3)
	{

	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
			return player->broadcast(u"You need to be in a room in order to use this command");

		auto roomSession = roomPlayer->getRoom()->getRoomSession();

		if (roomSession != nullptr)
			return player->broadcast(u"Can't force start a room while playing");

		const auto room = roomPlayer->getRoom();
		const auto players = room->getPlayers();

		for (const auto& [id, roomPlayer] : players)
			roomPlayer->setReady();

		room->start();
	}
};