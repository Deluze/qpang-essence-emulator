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
	DailyRewardCommand() : Command(3)
	{

	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer != nullptr) {
			player->broadcast(u"You may only claim your daily reward in the park.");

			return;
		}

		auto squarePlayer = player->getSquarePlayer();

		if (squarePlayer == nullptr)
		{
			player->broadcast(u"You may only claim your daily reward in the park.");

			return;
		}

		auto statement = DATABASE->prepare("SELECT * FROM daily_rewards WHERE player_id = ? LIMIT 1");

		statement->bindInteger(player->getId());

		auto result = statement->fetch();

		time_t currentTimeMillis = (time(0) * 1000);

		// Player does not exist in table, give player rewards and create record.
		if (!result->hasResults()) {
			giveDailyRewardToPlayer(player, 1, currentTimeMillis);

			auto insertStatement = DATABASE->prepare(
				"INSERT INTO daily_rewards (player_id, streak, last_activated) VALUES(?, 1, ?)"
			);

			insertStatement->bindInteger(player->getId());
			insertStatement->bindLong(currentTimeMillis);

			// Insert record for player daily reward.
			insertStatement->execute();

			return;
		}

		auto loginStreak = result->getInt("streak");
		auto lastActivated = result->getBigInt("last_activated");

		auto nextClaimDate = (lastActivated + (86400 * 1000));

		// Checks if the "nextClaimDate" is in the past.
		if (nextClaimDate < currentTimeMillis) {
			giveDailyRewardToPlayer(player, loginStreak, lastActivated);

			// TODO: Check if the login streak can continue.

			auto preparedStatement = DATABASE->prepare("UPDATE daily_rewards SET last_activated = ? WHERE player_id = ?");

			preparedStatement->bindInteger(currentTimeMillis);
			preparedStatement->bindInteger(player->getId());

			preparedStatement->execute();
		}
		else {
			auto timeLeft = (nextClaimDate - currentTimeMillis);

			auto hoursLeft = (int)floor((timeLeft / (1000 * 60 * 60)) % 24);
			auto minutesLeft = (int)floor((timeLeft / (1000 * 60)) % 60);
			auto secondsLeft = (int)floor((timeLeft / (1000)) % 60);

			char buffer[1000];

			std::cout << "Time left: " << hoursLeft << "h " << minutesLeft << "m " << secondsLeft << "s\n";

			sprintf_s(buffer, "You have recently claimed your daily rewards, "
				"you can re-claim your rewards in %i hours %i minutes and %i seconds.\n",
				hoursLeft, minutesLeft, secondsLeft);

			player->broadcast(StringConverter::Utf8ToUtf16(buffer));


			return;
		}
	}

	void giveDailyRewardToPlayer(std::shared_ptr<Player> player, uint32_t loginStreak, long lastActivated) {
		player->addCash(50);
		player->addDon(50);

		player->update();
		player->send(UpdateAccount(player));

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		player->broadcast(u"You have claimed your daily reward and have received 50 don and 50 cash.");

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		player->broadcast(u"Come back tomorrow to re-claim your reward and increase your login streak.");

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		player->broadcast(u"Re-enter the park to view your updated balance.");
	}
};