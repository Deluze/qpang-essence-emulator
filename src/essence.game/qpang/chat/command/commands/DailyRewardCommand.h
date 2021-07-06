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
		auto increaseStreakDate = (lastActivated + ((86400 * 1000) * 2));

		// Checks if the "nextClaimDate" is in the past.
		if (nextClaimDate < currentTimeMillis) {
			auto newLoginStreak = loginStreak;

			// Current date time is greater than last time activated and its less then the increase streak date time.
			if (currentTimeMillis >= lastActivated && currentTimeMillis <= increaseStreakDate) {
				newLoginStreak += 1;
			}
			else {
				newLoginStreak = 1;
			}

			giveDailyRewardToPlayer(player, newLoginStreak, lastActivated);

			auto preparedStatement = DATABASE->prepare(
				"UPDATE daily_rewards SET last_activated = ?, streak = ? WHERE player_id = ?"
			);

			preparedStatement->bindLong(currentTimeMillis);
			preparedStatement->bindInteger(newLoginStreak);
			preparedStatement->bindInteger(player->getId());

			preparedStatement->execute();
		}
		else {
			auto timeLeft = (nextClaimDate - currentTimeMillis);

			auto hoursLeft = (int)floor((timeLeft / (1000 * 60 * 60)) % 24);
			auto minutesLeft = (int)floor((timeLeft / (1000 * 60)) % 60);
			auto secondsLeft = (int)floor((timeLeft / (1000)) % 60);

			char buffer[1000];

			sprintf_s(buffer, "You can re-claim your rewards in %ih %im and %is.",
				hoursLeft, minutesLeft, secondsLeft);

			player->broadcast(
				u"\nYou have already claimed your daily rewards.\n" +
				StringConverter::Utf8ToUtf16(buffer)
			);

			return;
		}
	}

	void giveDailyRewardToPlayer(std::shared_ptr<Player> player, uint32_t loginStreak, long lastActivated) {
		auto amountOfCash = 100;
		auto amountOfDon = 100;
		auto amountOfGoldenCoins = 300;

		player->addCash(amountOfCash);
		player->addDon(amountOfDon);
		// 1 golden coin = 100, 3 = 300.
		player->addCoins(amountOfGoldenCoins);

		player->update();
		player->send(UpdateAccount(player));

		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		player->broadcast(
			u"\nYou have claimed your daily reward and have received 100 don, 100 cash and 2 golden coins.\n"
			u"Come back tomorrow to re-claim your reward and increase your login streak.\n"
			u"Re-enter the park to view your updated balance."
		);

		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//player->broadcast(u"Come back tomorrow to re-claim your reward and increase your login streak.");

		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//player->broadcast(u"Re-enter the park to view your updated balance.");
	}
};