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

		auto playerDailyRewardInfo = getDailyRewardInfoForPlayer(player->getId());

		auto currentTimeInMillis = (time(0) * 1000);

		// Player does not exist in table, give player rewards and create record.
		if (!playerDailyRewardInfo->hasResults()) {
			claimDailyReward(player, 1, currentTimeInMillis);

			DATABASE_DISPATCHER->dispatch(
				"INSERT INTO daily_rewards (player_id, streak, last_activated) VALUES(?, 1, ?)",
				{
					player->getId(),
					currentTimeInMillis
				});

			return;
		}

		auto dailyLoginStreak = playerDailyRewardInfo->getInt("streak");
		auto timeLastActivated = playerDailyRewardInfo->getBigInt("last_activated");

		auto nextDailyRewardClaimDate = (timeLastActivated + (86400 * 1000));
		auto timeUntillStreakCanBeIncreased = (timeLastActivated + ((86400 * 1000) * 2));

		// Checks if the "nextClaimDate" is in the past.
		if (nextDailyRewardClaimDate < currentTimeInMillis) {
			auto newLoginStreak = dailyLoginStreak;

			// Current date time is greater than last time activated and its less then the increase streak date time.
			if (currentTimeInMillis >= timeLastActivated && currentTimeInMillis <= timeUntillStreakCanBeIncreased) {
				newLoginStreak += 1;
			}
			else {
				newLoginStreak = 1;
			}

			claimDailyReward(player, newLoginStreak, timeLastActivated);

			DATABASE_DISPATCHER->dispatch(
				"UPDATE daily_rewards SET last_activated = ?, streak = ? WHERE player_id = ?",
				{
					currentTimeInMillis,
					newLoginStreak,
					player->getId()
				});
		}
		else {
			auto timeLeft = (nextDailyRewardClaimDate - currentTimeInMillis);

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

	/// <summary>
	/// Gets the daily reward info from the database for the given player.
	/// </summary>
	/// <param name="playerId">The id of the player.</param>
	StatementResult::Ptr getDailyRewardInfoForPlayer(uint32_t playerId)
	{
		auto statement = DATABASE->prepare("SELECT * FROM daily_rewards WHERE player_id = ? LIMIT 1");

		statement->bindInteger(playerId);

		return statement->fetch();
	}

	/// <summary>
	/// Gives the daily reward to the player and sends an update packet.
	/// </summary>
	/// <param name="player">The player in question.</param>
	/// <param name="loginStreak">The current login streak of the player.</param>
	/// <param name="lastActivated">The last time the player has claimed their reward.</param>
	void claimDailyReward(std::shared_ptr<Player> player, uint32_t loginStreak, long lastActivated)
	{
		auto amountOfCash = 100;
		auto amountOfDon = 100;
		auto amountOfGoldenCoins = 300;

		player->addCash(amountOfCash);
		player->addDon(amountOfDon);
		// 1 golden coin = 100, 3 = 300.
		player->addCoins(amountOfGoldenCoins);

		player->update();
		player->send(UpdateAccount(player));

		player->broadcast(
			u"\nYou have claimed your daily reward and have received 100 don, 100 cash and 2 golden coins.\n"
			u"Come back tomorrow to re-claim your reward and increase your login streak.\n"
			u"Re-enter the park to view your updated balance."
		);
	}
};