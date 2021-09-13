#pragma once

#include "qpang/player/Player.h"
#include "qpang/room/RoomPlayer.h"
#include "qpang/room/Room.h"
#include <qpang/Game.h>

#include "qpang/chat/command/Command.h"

#include "packets/lobby/outgoing/account/UpdateAccount.h"
#include "packets/lobby/outgoing/player/UpdateCashBalance.h"

#include <string.h>

class DailyRewardCommand : public Command
{
public:
	DailyRewardCommand() : Command(1)
	{

	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		player->broadcast(u"Please visit the bunny in the park to receive your daily bonus from now on!");
	}
};