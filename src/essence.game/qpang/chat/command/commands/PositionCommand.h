#pragma once

#include "qpang/chat/command/Command.h"

#include "qpang/room/RoomPlayer.h"
#include "qpang/room/tnl/net_events/server/gc_mesg.hpp"
#include "utils/StringConverter.h"

class PositionCommand : public Command
{
public:
	PositionCommand() : Command(4)
	{

	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
			return player->broadcast(u"You need to be in a room in order to use this command.");

		auto session = roomPlayer->getRoomSessionPlayer();

		if (session == nullptr)
			return player->broadcast(u"You need to be in a game to use this command.");

		const auto location = session->getPosition();

		char buffer[60];

		sprintf_s(buffer, "X: %f, Y: %f, Z: %f", location.x, location.y, location.z);
		
		player->broadcast(StringConverter::Utf8ToUtf16(buffer));
	}
};