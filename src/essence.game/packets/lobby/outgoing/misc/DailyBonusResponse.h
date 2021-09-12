#pragma once

#include "packets/LobbyServerPacket.h"
#include <qpang/player/Player.h>

class DailyBonusResponse : public LobbyServerPacket
{
public:
	DailyBonusResponse(const Player::Ptr& player, const bool isElegible) : LobbyServerPacket(902)
	{
		const auto currentCoinBalance = player->getCoins();

		if (isElegible) {
			// TODO: Increased chance for extra coins based on login streak?
			const auto hasExtraCoins = (rand() % 2) == 0;
			const auto amountOfExtraCoins = hasExtraCoins ? 200 : 0;

			const auto amountOfBonusCoins = (300 + amountOfExtraCoins);
			const auto updatedCoinBalance = (currentCoinBalance + amountOfBonusCoins);

			player->addCoins(amountOfBonusCoins);

			writeInt(amountOfBonusCoins);		// The amount of coins the player receives as a daily bonus.
			writeInt(updatedCoinBalance);		// The updated amount of coins for the player.
			writeInt(hasExtraCoins ? 1 : 0);	// If the player does not get extra coins, send 0, otherwise send 1.
		}
		else
		{
			writeInt(0);					// Send 0 if the player has already received their bonus today.
			writeInt(currentCoinBalance);	// The total amount of coins the player has.
			writeInt(0);					// does nothing, just send 0.
		}
	}
};