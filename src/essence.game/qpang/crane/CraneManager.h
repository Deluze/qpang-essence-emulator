#pragma once

#include <mutex>
#include <vector>

#include "qpang/crane/CraneItem.h"
#include "qpang/player/inventory/InventoryCard.h"

class CraneManager
{
public:
	void initialize();

	InventoryCard getRandomItem();
	bool isEnabled() const;
private:
	std::mutex m_mx;
	std::vector<CraneItem> m_items;
};