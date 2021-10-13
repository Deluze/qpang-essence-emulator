#pragma once

#include <array>
#include <memory>
#include <cstdint>

#include "qpang/player/inventory/InventoryCard.h"

class RoomSessionPlayer;

class PlayerSkillManager
{
public:
	void initialize(std::shared_ptr<RoomSessionPlayer> player);

	void tick();

	void removeSkillPoints(uint32_t amount = 100);
	void addSkillPoints(uint32_t amount = 100);
	void resetPoints();

	uint32_t drawSkill();
private:
	std::weak_ptr<RoomSessionPlayer> m_player;

	//std::shared_ptr<Skill> m_drawnSkillCard;
	//std::array<InventoryCard, 3> m_equippedCards;

	//std::shared_ptr<Skill> m_activeSkillCard;
	
	uint32_t m_skillPoints;
};