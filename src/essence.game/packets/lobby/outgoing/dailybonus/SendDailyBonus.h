#pragma once

#include "packets/LobbyServerPacket.h"
#include <qpang/player/Player.h>

#include "SendUpdatePlayerCashBalance.h"

class SendDailyBonus : public LobbyServerPacket
{
public:
	SendDailyBonus(const Player::Ptr& player, const bool isEligibleForDailyBonus) : LobbyServerPacket(902)
	{
		if (isEligibleForDailyBonus)
		{
			SendSuccess(player);

			player->send(SendUpdatePlayerCashBalance(player->getCash()));
		}
		else
		{
			// Player has already received their reward.
			SendFail(player);
		}
	}

private:
	void SendSuccess(const Player::Ptr& player)
	{
		const auto playerIsPatreon = player->isPatreon();

		// Patreons have more chance to get extra rewards.
		const auto hasExtraRewards = playerIsPatreon
			? (rand() % 100 <= 33)
			: (rand() % 100 <= 20);

		const auto extraCoins = hasExtraRewards ? 200 : 0;

		// 1/100 for bronze, 10/100 for silver and 100/100 for gold.
		const auto coinBonus = 100 + extraCoins;

		// Add the coin bonus.
		player->addCoins(coinBonus);

		const auto extraCash = hasExtraRewards ? 30 : 0;
		const auto cashBonus = 60 + extraCash;

		const auto extraDon = hasExtraRewards ? 600 : 0;
		const auto donBonus = 1200 + extraDon;

		// Add the cash and don bonus.
		player->addCash(cashBonus);
		player->addDon(donBonus);

		const auto updatedCoinBalance = player->getCoins();

		// Send the amount of coins the player receives as a bonus.
		writeInt(coinBonus);
		// Send the updated coin balance of the player (current amount of coins + bonus).
		writeInt(updatedCoinBalance);
		// Send "true" indicating the player gets extra rewards on top of the bonus, send "false" indicating the player does not get extra rewards.
		writeFlag(hasExtraRewards);
	}

	void SendFail(const Player::Ptr& player)
	{
		const auto currentCoinBalance = player->getCoins();

		// Send "0" indicating the player does not get any coins.
		writeInt(0);
		// Send the current amount of coins the player has.
		writeInt(currentCoinBalance);
		// Just send "false", changing this value does not do anything (afaik).
		writeFlag(false);
	}
};
