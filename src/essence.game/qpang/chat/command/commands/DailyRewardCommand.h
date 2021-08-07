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
					static_cast<uint64_t>(currentTimeInMillis)
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
					static_cast<uint64_t>(currentTimeInMillis),
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
				u"\nYou have recently claimed your daily rewards.\n" +
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
		auto playerHasIncreasedReward = player->hasIncreasedDailyReward();

		auto baseAmountOfCash = 50;
		auto baseAmountOfDon = 1000;

		if (playerHasIncreasedReward)
		{
			baseAmountOfCash = 60;
			baseAmountOfDon = 1200;
		}

		if (loginStreak > 7) 
		{
			loginStreak = 7;
		}

		float rewardModifier = (loginStreak / static_cast<float>(10));

		int dailyRewardCash = static_cast<int>(baseAmountOfCash * (1 + rewardModifier));
		int dailyRewardDon = static_cast<int>(baseAmountOfDon * (1 + rewardModifier));
		
		player->addCash(dailyRewardCash);
		player->addDon(dailyRewardDon);

		player->update();
		player->send(UpdateAccount(player));

		char buffer[1000];

		sprintf_s(buffer, 
			"\nYou have claimed your daily reward and have received %i cash and %i don.\n"
			"Your current login-streak is now at %i.\n"
			"Re-enter the park to view your updated balance.", 
			(int) dailyRewardCash, (int) dailyRewardDon, loginStreak
		);

		player->broadcast(StringConverter::Utf8ToUtf16(buffer));
	}
};