#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

#include "qpang/room/session/RoomSession.h"
#include "qpang/room/RoomPlayer.h"

class ToggleReloadGlitchCommand : public Command
{
public:
	ToggleReloadGlitchCommand() : Command(3)
	{

	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			player->broadcast(u"You need to be in a room in order to use this command!");

			return;
		}

		if (roomPlayer->getRoom()->isReloadGlitchEnabled()) {
			roomPlayer->getRoom()->setIsReloadGlitchEnabled(false);

			for (const auto& [id, player] : roomPlayer->getRoom()->getPlayers())
			{
				player->getPlayer()->broadcast(u"The reload glitch has been DISABLED for this room.");
			}

			return;
		}
		else
		{
			roomPlayer->getRoom()->setIsReloadGlitchEnabled(true);

			for (const auto& [id, player] : roomPlayer->getRoom()->getPlayers())
			{
				player->getPlayer()->broadcast(u"The reload glitch has been ENABLED for this room.");
			}

			return;
		}
	}
};