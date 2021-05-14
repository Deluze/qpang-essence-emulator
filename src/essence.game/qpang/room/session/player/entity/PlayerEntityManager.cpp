#include "PlayerEntityManager.h"

#include "qpang/room/session/player/RoomSessionPlayer.h"

void PlayerEntityManager::initialize(std::shared_ptr<RoomSessionPlayer> player)
{
	m_player = player;
}

void PlayerEntityManager::shoot(uint32_t entityId)
{	
	m_validBullets[m_bulletIndex] = PlayerBulletEntity(entityId);

	if (m_bulletIndex == 19)
		m_bulletIndex = 0;
	else
		m_bulletIndex++;
}

bool PlayerEntityManager::isValidShot(uint32_t entityId)
{
	for (size_t i = 0; i < 20; i++)
	{
		auto entity = m_validBullets[i];

		if (entity.getId() == entityId)
			return true;
	}

	return false;
}

void PlayerEntityManager::addKill(uint32_t entityId)
{
	auto player = m_player.lock();

	if (player == nullptr)
		return;
	
	for (size_t i = 0; i < 20; i++)
	{
		auto* entity = &m_validBullets[i];

		if (entity->getId() == entityId)
		{
			entity->addKill();

			const auto killCount = entity->getKillCount();

			if (killCount > player->getHighestMultiKill())
				player->setHighestMultiKill(killCount);
			
			return;
		}
	}
}

void PlayerEntityManager::close()
{

	auto player = m_player.lock();

	if (player == nullptr)
		return;

	const auto highestMultiKill = player->getHighestMultiKill();

	for (size_t i = 0; i < 20; i++)
	{
		auto entity = m_validBullets[i];
		const auto killCount = entity.getKillCount();
		
		if (killCount > highestMultiKill)
			player->setHighestMultiKill(killCount);
	}
}
