#pragma once

#include "gc_pve_npc_init.hpp"
#include "qpang/chat/command/Command.h"
#include "qpang/room/RoomPlayer.h"

class SpawnNpcCommand final : public Command
{
public:
	SpawnNpcCommand() : Command(3)
	{
	}

	std::vector<CommandArgument*> getArguments() override
	{
		return {
			argTypes[INTEGER]
		};
	}

	void handle(const std::shared_ptr<Player> player, const std::vector<std::u16string>& args) override
	{
		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			player->broadcast(u"You need to be in a room in order to use this command.");

			return;
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			player->broadcast(u"You need to be in a game in order to use this command.");

			return;
		}

		const auto roomSession = roomSessionPlayer->getRoomSession();

		roomSession->getNpcManager()->spawnInitialNpcs();
	}
};