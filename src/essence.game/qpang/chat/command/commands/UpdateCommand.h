#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

#include "qpang/player/Player.h"
#include "packets/lobby/outgoing/account/UpdateAccount.h"

class UpdateCommand : public Command
{
public:
	UpdateCommand() : Command(3)
	{

	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		player->send(UpdateAccount{player});

		player->broadcast(u"Updated account.");
	}
};