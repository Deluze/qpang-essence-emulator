#include "RedeemCodeManager.h"

#include "Database.h"
#include "Emulator.h"
#include "SendUseRedeemCodeError.h"
#include "SendUseRedeemCodeSuccess.h"

bool RedeemCodeManager::isOfValidLength(const std::u16string& redeemCode)
{
	return (redeemCode.length() == 14);
}

RedeemCode RedeemCodeManager::tryGetRedeemCodeForPlayer(const std::u16string& redeemCode, const uint32_t playerId)
{
	const auto statement = DATABASE->prepare("																					\
		SELECT `redeem_codes`.* FROM `redeem_codes`																				\
			LEFT JOIN `redeem_code_uses`																						\
			ON `redeem_code_uses`.`redeem_code_id` = `redeem_codes`.`id` AND `redeem_code_uses`.`player_id` = ?					\
		WHERE (`redeem_codes`.`redeem_code` = ?)																				\
			AND (`redeem_codes`.`player_id` = ? OR (`redeem_codes`.`player_id` = 0 AND ISNULL(`redeem_code_uses`.`use_count`)))	\
			AND	(`redeem_codes`.`total_uses` = 0 OR `redeem_codes`.`use_count` < `redeem_codes`.`total_uses`)					\
			AND	(`redeem_codes`.`active` = 1)																					\
		LIMIT 1																													\
	");

	statement->bindInteger(playerId);
	statement->bindString(redeemCode);
	statement->bindInteger(playerId);

	const auto result = statement->fetch();

	if (!result->hasResults())
	{
		return {};
	}

	return RedeemCode{
		result->getInt("id"),
		result->getBigInt("player_id"),
		result->getString("redeem_code"),
		result->getInt("total_uses"),
		result->getInt("use_count"),
		result->getInt("cash"),
		result->getInt("don"),
		result->getFlag("active")
	};
}

std::vector<InventoryCard> RedeemCodeManager::getInventoryCardsForRedeemCode(const uint32_t redeemCodeId)
{
	const auto statement = DATABASE->prepare("																			\
		SELECT `redeem_cards`.* FROM `redeem_codes`																		\
			INNER JOIN `redeem_code_redeem_cards` ON `redeem_code_redeem_cards`.`redeem_code_id` = `redeem_codes`.`id`	\
			INNER JOIN `redeem_cards` ON `redeem_cards`.`id` = `redeem_code_redeem_cards`.`redeem_card_id`				\
		WHERE `redeem_codes`.`id` = ?																					\
	");

	statement->bindInteger(redeemCodeId);

	const auto result = statement->fetch();

	if (!result->hasResults())
	{
		return {};
	}

	std::vector<InventoryCard> inventoryCards = {};

	while (result->hasNext())
	{
		const auto inventoryCard = InventoryCard{
			0,
			0,
			result->getInt("item_id"),
			result->getTiny("type"),
			result->getTiny("period_type"),
			result->getTiny("period"),
			false,
			false,
			result->getFlag("is_giftable"),
			0,
			0
		};

		inventoryCards.push_back(inventoryCard);

		result->next();
	}

	return inventoryCards;
}

void RedeemCodeManager::updateRedeemCodeUseCountForPlayer(const uint32_t redeemCodeId, const uint32_t playerId)
{
	const auto updateStatement = DATABASE->prepare("		\
		UPDATE `redeem_codes` SET							\
			`use_count` = (`use_count` + 1),				\
			`active` = IF(`use_count` = `total_uses`, 0, 1)	\
		WHERE `id` = ?										\
	");

	updateStatement->bindInteger(redeemCodeId);
	updateStatement->execute();

	const auto insertStatement = DATABASE->prepare("									\
  		INSERT INTO `redeem_code_uses` (`redeem_code_id`, `player_id`, `use_count`)		\
		VALUES (?, ?, 1) ON DUPLICATE KEY UPDATE `use_count` = (`use_count` + 1)		\
	");

	insertStatement->bindInteger(redeemCodeId);
	insertStatement->bindLong(playerId);

	insertStatement->execute();
}

void RedeemCodeManager::sendUseRedeemCodeFailToPlayer(const Player::Ptr& player, const uint32_t failCode)
{
	player->send(SendUseRedeemCodeError(failCode));
}

void RedeemCodeManager::sendRedeemCodeRewardsToPlayer(const Player::Ptr& player, const RedeemCode& redeemCode,
                                                      std::vector<InventoryCard> inventoryCards)
{
	updateRedeemCodeUseCountForPlayer(redeemCode.id, player->getId());

	player->addCash(redeemCode.cash);
	player->addDon(redeemCode.don);

	for (auto& inventoryCard : inventoryCards)
	{
		// ReSharper disable once CppUseStructuredBinding
		const auto createdInventoryCard = player->getInventoryManager()->storeCard(inventoryCard);

		inventoryCard.id = createdInventoryCard.id;
		inventoryCard.playerOwnerId = createdInventoryCard.playerOwnerId;
	}

	player->send(SendUseRedeemCodeSuccess(player->getDon(), player->getCash(), inventoryCards));
}
