#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "dailybonus/SendDailyBonus.h"

class HandleGetDailyBonusRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto player = conn->getPlayer();
		
		const auto playerDailyBonusRedeemedAtTime = getPlayerDailyBonusRedeemedAtTime(player->getId());

		if (playerDailyBonusRedeemedAtTime == -1)
		{
			player->broadcast(u"Something went wrong whilst attempting to retrieve your daily bonus. Please contact one of the game masters.");

			return;
		}

		const auto redeemedAtTimeInUtc = *localtime(&playerDailyBonusRedeemedAtTime);

		const auto now = time(nullptr);
		const auto nowInUtc = *localtime(&now);

		const auto nowYear = (nowInUtc.tm_year + 1900);
		const auto nowMonth = (nowInUtc.tm_mon + 1);
		const auto nowDay = (nowInUtc.tm_mday);

		const auto lastRedeemedYear = (redeemedAtTimeInUtc.tm_year + 1900);
		const auto lastRedeemedMonth = (redeemedAtTimeInUtc.tm_mon + 1);
		const auto lastRedeemedDay = (redeemedAtTimeInUtc.tm_mday);

		const auto isEligibleForDailyBonus = (nowDay > lastRedeemedDay) || (nowMonth > lastRedeemedMonth) || (nowYear > lastRedeemedYear);

		if (isEligibleForDailyBonus)
		{
			setPlayerDailyBonusRedeemedAtTime(player->getId(), now);	
		}

		player->send(SendDailyBonus(player, isEligibleForDailyBonus));
	}

private:
	static void setPlayerDailyBonusRedeemedAtTime(const uint32_t playerId, const uint64_t time) 
	{
		const auto statement = DATABASE->prepare("UPDATE players SET daily_bonus_redeemed_at = ? WHERE id = ?");

		statement->bindLong(time);
		statement->bindInteger(playerId);

		statement->execute();
	}

	static time_t getPlayerDailyBonusRedeemedAtTime(const uint32_t playerId) 
	{
		const auto statement = DATABASE->prepare("SELECT daily_bonus_redeemed_at FROM players WHERE id = ? LIMIT 1");

		statement->bindInteger(playerId);

		const auto result = statement->fetch();

		if (!result->hasResults()) 
		{
			return -1;
		}

		return result->getBigInt("daily_bonus_redeemed_at");
	}
};
