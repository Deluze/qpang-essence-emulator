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
	void initialize(std::shared_ptr<RoomSession> room);

	std::shared_ptr<Skill> generateRandomSkill();
private:
	std::weak_ptr<RoomSession> m_roomSession;
	std::vector<std::function<std::unique_ptr<Skill>()>> m_skills;
};