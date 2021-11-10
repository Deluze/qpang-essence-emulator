#pragma once

#include "Player.h"
#include "UseRedeemCodeFailResponse.h"
#include "UseRedeemCodeSuccessResponse.h"
#include "core/communication/packet/PacketEvent.h"

class UseRedeemCodeRequest final : public PacketEvent
{
public:
	// NOTE: Struct of the redeem_codes table.
	struct RedeemCode
	{
		uint32_t id;
		std::u16string redeemCode;
		uint32_t totalUses;
		uint32_t useCount;
		uint64_t playerId;
		uint32_t cash;
		uint32_t don;
		bool isActive;
	};

	// TODO: Find out error codes for redeem failure.
	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto player = conn->getPlayer();
		const auto playerId = player->getId();

		const auto redeemCodeChars = pack.readArray<char, 14>();
		const auto code = std::u16string(redeemCodeChars.cbegin(), redeemCodeChars.cend());

		const auto redeemCode = tryGetRedeemCode(code, player);

		if (redeemCode.id == NULL)
		{
			conn->send(UseRedeemCodeFailResponse());

			return;
		}

		auto redeemCodeCards = getRedeemCodeCards(redeemCode.id, playerId);

		if (!player->getInventoryManager()->hasSpace(redeemCodeCards.size()) && !redeemCodeCards.empty())
		{
			conn->send(UseRedeemCodeFailResponse());

			player->broadcast(u"You do not have enough inventory space left to redeem this code.");

			return;
		}

		updateRedeemCodeDataInDatabase(redeemCode.id, playerId);

		player->addCash(redeemCode.cash);
		player->addDon(redeemCode.don);

		for (auto& redeemCodeCard : redeemCodeCards)
		{
			// ReSharper disable once CppUseStructuredBinding
			const auto createdInventoryCard = player->getInventoryManager()->storeCard(redeemCodeCard);

			redeemCodeCard.id = createdInventoryCard.id;
			redeemCodeCard.playerOwnerId = createdInventoryCard.playerOwnerId;
		}

		conn->send(UseRedeemCodeSuccessResponse(player->getDon(), player->getCash(), redeemCodeCards));
	}
private:
	static RedeemCode tryGetRedeemCode(const std::u16string& enteredRedeemCode, const Player::Ptr& player)
	{	
		const auto statement = DATABASE->prepare("						\
			SELECT rc.* FROM `redeem_codes` AS rc												\
				LEFT JOIN `redeem_code_uses` rcu												\
				ON rcu.`redeem_code_id` = rc.`id` AND rcu.`player_id` = ?						\
			WHERE																				\
				(rc.`redeem_code` = ?)				 										AND	\
				(rc.`player_id` = ? OR (rc.`player_id` = 0 AND ISNULL(rcu.`use_count`))) 	AND	\
				(rc.`total_uses` = 0 OR rc.`use_count` < rc.`total_uses`)					AND	\
				(rc.`active` = 1)																\
			LIMIT 1																				\
		");

		statement->bindInteger(player->getId());
		statement->bindString(enteredRedeemCode);
		statement->bindInteger(player->getId());

		const auto redeemCodesResult = statement->fetch();

		if (!redeemCodesResult->hasResults())
		{
			return RedeemCode{};
		}

		// ReSharper disable once CppUseStructuredBinding
		return RedeemCode
		{
			redeemCodesResult->getInt("id"),
			redeemCodesResult->getString("redeem_code"),
			redeemCodesResult->getInt("total_uses"),
			redeemCodesResult->getInt("use_count"),
			redeemCodesResult->getBigInt("player_id"),
			redeemCodesResult->getInt("cash"),
			redeemCodesResult->getInt("don"),
			redeemCodesResult->getFlag("active"),
		};
	}

	static void updateRedeemCodeDataInDatabase(const uint32_t redeemCodeId, const uint32_t playerId)
	{
		const auto updateRedeemCodeStatement = DATABASE->prepare("\
			UPDATE `redeem_codes`														\
			SET `use_count` = (`use_count` + 1),										\
				`active`	= IF(`use_count` = `total_uses`, 0, 1)						\
			WHERE `id` = ?																\
		");

		updateRedeemCodeStatement->bindInteger(redeemCodeId);
		updateRedeemCodeStatement->execute();

		const auto updateRedeemCodeUsesStatement = DATABASE->prepare("\
  			INSERT INTO `redeem_code_uses` (`redeem_code_id`, `player_id`, `use_count`)		\
			VALUES (?, ?, 1) ON DUPLICATE KEY UPDATE `use_count` = (`use_count` + 1)		\
		");

		updateRedeemCodeUsesStatement->bindInteger(redeemCodeId);
		updateRedeemCodeUsesStatement->bindLong(playerId);

		updateRedeemCodeUsesStatement->execute();
	}

	static std::vector<InventoryCard> getRedeemCodeCards(const uint32_t redeemCodeId, const uint32_t playerId)
	{
		const auto statement = DATABASE->prepare("SELECT * FROM `redeem_code_cards` WHERE `redeem_code_id` = ?");

		statement->bindInteger(redeemCodeId);

		const auto result = statement->fetch();

		if (!result->hasResults())
		{
			return {};
		}

		std::vector<InventoryCard> inventoryCards = {};

		while (result->hasNext())
		{
			auto inventoryCard = InventoryCard{
				0,
				playerId,
				result->getInt("item_id"),
				result->getTiny("type"),
				static_cast<uint8_t>(result->getInt("period_type")),
				static_cast<uint16_t>(result->getInt("period")),
				false,
				false,
				result->getFlag("giftable"),
				0,
				0
			};

			inventoryCards.push_back(inventoryCard);

			result->next();
		}

		return inventoryCards;
	}
};