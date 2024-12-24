#pragma once

#include <memory>
#include <unordered_map>
#include <mutex>

#include "core/Emulator.h"
#include "qpang/player/inventory/InventoryCard.h"

class Player;

class InventoryManager
{
public:
	void initialize(const std::shared_ptr<Player>& player, uint32_t playerId);
	std::vector<InventoryCard> list();
	void sendCards();
	void sendGifts();
	std::vector<InventoryCard> listGifts();
	InventoryCard get(uint64_t cardId);

	
	bool hasCard(const uint64_t cardId);
	void deleteCard(const uint64_t cardId);
	void setCardActive(const uint64_t cardId, bool isActive = true);
	bool addCard(const InventoryCard& card);
	InventoryCard storeCard(InventoryCard& card);
	void useSkillCard(uint64_t cardId, uint16_t period);
	void useCard(const uint64_t cardId, const uint32_t period = 0);
	bool isExpired(const uint64_t cardId);
	bool hasSpace(uint32_t space) const;
	bool hasSpace() const;

	void tradeCard(InventoryCard& card, const std::shared_ptr<Player>& player);

	void giftCard(InventoryCard& card, const std::shared_ptr<Player>& player);
	void receiveGift(InventoryCard& card, const std::u16string& sender);
	void openGift(uint64_t cardId);

	bool hasGiftSpace() const;

	void openCard(uint64_t cardId);

	void close();
private:
	std::weak_ptr<Player> m_player;
	std::mutex m_mx;

	std::unordered_map<uint64_t, InventoryCard> m_cards;
	std::unordered_map<uint64_t, InventoryCard> m_gifts;

	template<typename T>
	std::vector<T> slice(std::vector<T> const& v, uint32_t m, uint32_t n)
	{
		auto first = v.cbegin() + m;
		auto last = v.cbegin() + n + 1;

		std::vector<T> vec(first, last);
		return vec;
	}
};