#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include "Skill.h"

class RoomSession;
class RoomSessionPlayer;

class RoomSessionSkillManager
{
public:
	void initialize(const std::shared_ptr<RoomSession>& roomSession);
	std::shared_ptr<Skill> getSkillByItemId(const uint32_t itemId);

	std::shared_ptr<Skill> generateRandomSkill();

	bool isValidSkillForGameMode(uint32_t itemId);
private:
	std::weak_ptr<RoomSession> m_roomSession;
	std::vector<std::function<std::unique_ptr<Skill>()>> m_skillsForGameMode;
};