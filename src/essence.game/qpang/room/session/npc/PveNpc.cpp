#include "PveNpc.h"

#include "RoomSession.h"

// TODO: Create a more user-friendly constructor by making structs for the parameters.
// TODO: Add item drops parameter to the constructor so every npc can have custom loot & probability.
PveNpc::PveNpc(const eNpcType type, const Position& initialSpawnPosition, const uint16_t baseHealth,
	const uint16_t initialSpawnRotation, const bool canDropLootOnDeath, const bool shouldRespawn, const uint64_t respawnTime) :
	m_type(type),
	m_position(initialSpawnPosition),
	m_initialSpawnPosition(initialSpawnPosition),
	m_health(baseHealth),
	m_baseHealth(baseHealth),
	m_initialSpawnRotation(initialSpawnRotation),
	m_canDropLootOnDeath(canDropLootOnDeath),
	m_shouldRespawn(shouldRespawn),
	m_respawnTime(respawnTime)
{
}

void PveNpc::tick(const std::shared_ptr<RoomSession>& roomSession)
{
	if (isDead() && m_shouldRespawn)
	{
		const auto currentTime = time(nullptr);

		if (m_timeOfDeath == NULL)
		{
			return;
		}

		if ((m_timeOfDeath + m_respawnTime) < currentTime)
		{
			// Reset time of death.
			m_timeOfDeath = NULL;

			roomSession->getNpcManager()->respawnNpcByUid(m_uid);
		}
	}
}

void PveNpc::setUid(const uint32_t uid)
{
	m_uid = uid;
}

void PveNpc::resetHealth()
{
	m_health = m_baseHealth;
}

uint16_t PveNpc::takeDamage(const uint16_t damage)
{
	if (damage > m_health)
	{
		m_health = 0;

		return (damage - m_health);
	}

	m_health = m_health - damage;

	return damage;
}

void PveNpc::onDeath(const std::shared_ptr<RoomSession>& roomSession)
{
	if (m_shouldRespawn)
	{
		m_timeOfDeath = time(nullptr);
	}

	if (m_canDropLootOnDeath)
	{
		dropLoot(roomSession);
	}
}

uint32_t PveNpc::getUid() const
{
	return m_uid;
}

eNpcType PveNpc::getType() const
{
	return m_type;
}

Position PveNpc::getPosition() const
{
	return m_position;
}

Position PveNpc::getInitialSpawnPosition() const
{
	return m_initialSpawnPosition;
}

uint16_t PveNpc::getHealth() const
{
	return m_health;
}

uint16_t PveNpc::getBaseHealth() const
{
	return m_baseHealth;
}

uint16_t PveNpc::getInitialSpawnRotation() const
{
	return m_initialSpawnRotation;
}

bool PveNpc::shouldRespawn() const
{
	return m_shouldRespawn;
}

bool PveNpc::isDead() const
{
	return m_health <= 0;
}

void PveNpc::dropLoot(const std::shared_ptr<RoomSession>& roomSession) const
{
	std::vector availableItemDrops
	{
		ItemDrop{ eItemType::NONE, 45 },
		ItemDrop{ eItemType::AMMO_CLIP, 20 },
		ItemDrop{ eItemType::GOLDEN_COIN, 5 },
		ItemDrop{ eItemType::SILVER_COIN, 10 },
		ItemDrop{ eItemType::BRONZE_COIN, 20 }
	};

	std::vector<eItemType> itemDrops{};

	for (const auto [itemType, probability] : availableItemDrops)
	{
		for (size_t i = 0; i < probability; i++)
		{
			itemDrops.push_back(itemType);
		}
	}

	// TODO: Properly create a random seed.
	// See: https://stackoverflow.com/questions/50662280/c-need-a-good-technique-for-seeding-rand-that-does-not-use-time
	srand(time(nullptr));

	const auto randomItemType = itemDrops[rand() % itemDrops.size()];
	const auto randomPveItem = PveItem
	{
		randomItemType,
		m_position
	};

	roomSession->getPveItemManager()->spawnItem(randomPveItem);
}
