#pragma once
#include <string>
#include <vector>

#include "InventoryCard.h"
#include "Player.h"
#include "RedeemCode.h"

class RedeemCodeManager
{
public:
	/// <summary>
	/// Checks if a redeem code is of valid length, and returns the result.
	/// </summary>
	static bool isOfValidLength(const std::u16string& redeemCode);

	/// <summary>
	/// Tries to get the redeem code from the database for the given player when all conditions match.
	/// </summary>
	static RedeemCode tryGetRedeemCodeForPlayer(const std::u16string& redeemCode, const uint32_t playerId);

	/// <summary>
	/// Retrieves the inventory cards from the database that belong to the given redeem code.
	/// </summary>
	static std::vector<InventoryCard> getInventoryCardsForRedeemCode(const uint32_t redeemCodeId);

	/// <summary>
	/// Updates the redeem code use count, and gives the cash/don/inventory cards to the player.
	/// </summary>
	static void sendRedeemCodeRewardsToPlayer(const Player::Ptr& player, const RedeemCode& redeemCode, std::vector<InventoryCard> inventoryCards);
private:
	/// <summary>
	/// Updates the redeem code use count for the player in the redeem_codes and redeem_code_uses db tables.
	/// </summary>
	static void updateRedeemCodeUseCountForPlayer(const uint32_t redeemCodeId, const uint32_t playerId);
};
