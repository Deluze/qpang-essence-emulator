#pragma once

#include "qpang/player/Player.h"
#include "qpang/room/RoomPlayer.h"
#include "qpang/room/Room.h"
#include <qpang/Game.h>

#include "qpang/chat/command/Command.h"

class DailyRewardCommand : public Command
{
public:
	DailyRewardCommand() : Command(3)
	{

	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		player->broadcast(u"Work in progress");

		return;

		//// Check if player is in room.
		//auto roomPlayer = player->getRoomPlayer();

		//if (roomPlayer != nullptr) {
		//	player->broadcast(u"You may only retrieve your daily rewards in the square.");

		//	return;
		//}

		//auto squarePlayer = player->getSquarePlayer();

		//if (squarePlayer == nullptr)
		//{
		//	player->broadcast(u"You may only retrieve your daily rewards in the square.");

		//	return;
		//}

		//// TODO: Check dailyreward cooldown
		//auto isElegibleForDailyRewardStatement = DATABASE->prepare(
		//	"SELECT COUNT(*) FROM daily_rewards WHERE player_id = ? AND (last_activated + 86400) > ?"
		//);

		//isElegibleForDailyRewardStatement->bindInteger(player->getId());
		//isElegibleForDailyRewardStatement->bindInteger(time(NULL));

		//auto isElegibleForDailyReward = isElegibleForDailyRewardStatement->fetch()->hasResults();

		//if (isElegibleForDailyReward)
		//{
		//	player->broadcast(u"You are not elegible for the daily reward yet.");

		//	return;
		//}

		//player->broadcast(u"You are elegible for the daily reward.");
		//// TODO: Check dailyreward streak.
	}
};