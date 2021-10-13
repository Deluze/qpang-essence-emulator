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
private:
	std::weak_ptr<RoomSession> m_room;
	std::unordered_map<uint32_t, std::shared_ptr<Skill>> m_skillsForMode;
};