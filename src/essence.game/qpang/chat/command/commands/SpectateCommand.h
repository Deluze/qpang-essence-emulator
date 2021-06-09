#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

#include "qpang/player/Player.h"

class SpectateCommand : public Command
{
public:
	SpectateCommand() : Command(4)
	{

	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		auto target = args.empty() || args.at(0).empty() ? player : Game::instance()->getPlayer(args.at(0));

		if (target == nullptr)
		{
			player->broadcast(u"This player does not exist");
			return;
		}

		std::u16string refer = target == player ? u"You" : u"Target player";
		auto roomPlayer = target->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			player->broadcast(refer + u" must be in a room in order to use this command.");
			return;
		}

		if (roomPlayer->isPlaying())
		{
			player->broadcast(refer + u" must not be in a game in order to use this command.");
			return;
		}

		if (roomPlayer->isReady())
		{
			player->broadcast(refer + u" must not be ready in order to use this command.");
			return;
		}

		auto room = roomPlayer->getRoom();

		if (!room->isPlaying())
		{
			player->broadcast(u"The game must be in session in order to spectate.");
			return;
		}

		/*
		if (room->isEventRoom())
		{
			player->broadcast(u"No! You may not spectate event rooms");
			return;
		}
		*/

		roomPlayer->setReady(true);
		roomPlayer->setPlaying(true);
		roomPlayer->setSpectating(true);
		roomPlayer->getConnection()->startSpectating(room, roomPlayer);
		roomPlayer->onStart();
	}
};