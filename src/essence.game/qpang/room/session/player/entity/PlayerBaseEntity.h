#pragma once

#include <cstdint>
#include <vector>

class PlayerBaseEntity
{
public:
	PlayerBaseEntity(uint32_t id = 0);

	uint32_t getId();
	uint8_t getKillCount();
	bool isPlayerValidForHit(uint32_t playerId);
	
	void addKill();
	void onPlayerHit(uint32_t playerId);
private:
	std::vector<uint32_t> m_hitPlayers;
	uint8_t m_killCount;
	uint32_t m_id;
};