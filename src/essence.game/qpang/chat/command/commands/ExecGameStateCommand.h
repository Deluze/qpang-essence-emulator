#pragma once

#include "qpang/player/Player.h"
#include "qpang/room/RoomPlayer.h"
#include "qpang/room/Room.h"
#include "qpang/room/session/RoomSession.h"
#include <qpang/Game.h>

#include "qpang/chat/command/Command.h"

#include <string.h>
#include <qpang/room/tnl/net_events/server/gc_game_state.hpp>
#include <qpang/room/tnl/net_events/server/gc_card.hpp>

class ExecGameStateCommand : public Command
{
public:
	ExecGameStateCommand() : Command(3)
	{

	}

	std::vector<CommandArgument*> getArguments() override
	{
		return {
			Command::argTypes[Command::Validation::INTEGER],
			Command::argTypes[Command::Validation::INTEGER],
		};
	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			player->broadcast(u"");

			return;
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			player->broadcast(u"Go in a game.");

			return;
		}

		const auto roomSession = roomSessionPlayer->getRoomSession();

		if (roomSession == nullptr)
		{
			player->broadcast(u"Go in a game.");

			return;
		}

		const auto cmd = convertToInteger(args.at(0));

		const auto uid = player->getId();
		const auto targetUid = uid;

		const auto cardType = 7;

		const auto itemId = convertToInteger(args.at(1));
		const auto seqId = itemId;

		roomSession->relayPlaying<GCCard>(uid, targetUid, cmd, cardType, itemId, seqId);
		//roomSession->relayPlaying<GCCard>(uid, targetUid, cmd, cardType, itemId, seqId);


		std::cout << "ExecGameStateCommand::Handle >> Sending GCCard.\n";
	}
};