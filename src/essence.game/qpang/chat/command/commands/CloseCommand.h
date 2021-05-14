#pragma once

#include "qpang/chat/command/Command.h"

#include "qpang/player/Player.h"
#include "qpang/room/RoomPlayer.h"
#include "qpang/room/Room.h"

class CloseCommand : public Command
{
public:
	CloseCommand() : Command(4)
	{

	}
	
	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
			return;

		auto room = roomPlayer->getRoom();
		room->close();
	}
};