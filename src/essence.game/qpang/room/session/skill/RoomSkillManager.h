#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include "Skill.h"

class RoomSession;
class RoomSessionPlayer;

class RoomSkillManager
{
public:
	void initialize(std::shared_ptr<RoomSession> room);

	std::shared_ptr<Skill> generateRandomSkill();
	std::shared_ptr<Skill> getSkillByItemId(uint32_t skillItemId);

	bool isValidSkillCard(uint32_t skillItemId);
private:
	std::weak_ptr<RoomSession> m_room;
	std::unordered_map<uint32_t, std::function<std::unique_ptr<Skill>()>> m_skills;
};