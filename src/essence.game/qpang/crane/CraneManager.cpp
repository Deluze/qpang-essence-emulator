#include "CraneManager.h"

#include "core/Emulator.h"
#include "core/database/Database.h"

void CraneManager::initialize()
{
	srand(time(nullptr));

	std::lock_guard<std::mutex> l(m_mx);

	m_items.clear();
	
	auto res = DATABASE->prepare("SELECT * FROM crane_items WHERE active = 1")->fetch();

	if (!res->hasResults())
	{
		return;
	}
	
	while (res->hasNext())
	{
		uint32_t itemId = res->getInt("item_id");
		uint8_t type = res->getTiny("item_type");
		uint32_t period = res->getInt("period");

		CraneItem item;

		item.itemId = itemId;
		item.itemType = type;
		item.period = period;

		m_items.push_back(item);
		
		res->next();
	}
}

InventoryCard CraneManager::getRandomItem()
{
	std::lock_guard<std::mutex> l(m_mx);

	const auto item = m_items.at(rand() % m_items.size());

	InventoryCard card;
	card.itemId = item.itemId;
	card.type = item.itemType;
	card.isGiftable = true;

	const auto isUnlimited = rand() % 20 == 0;

	if (isUnlimited)
	{
		card.periodType = 254;
		card.period = 1;
	}
	else
	{
		card.periodType = 3;
		card.period = (rand() % (item.period / 2)) + item.period;
	}

	return card;
}

bool CraneManager::isEnabled() const
{
	return !m_items.empty();
}
