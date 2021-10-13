#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

//class Skill;
class RoomSession;
class RoomSessionPlayer;

class RoomSkillManager
{
public:
	void initialize(std::shared_ptr<RoomSession> room);

	//std::shared_ptr<Skill> generateRandomSkill();

	//bool isSkillValid(uint32_t itemId);
private:
	//std::unordered_map<uint32_t, std::function<std::unique_ptr<Skill>()>> m_skills;
	std::weak_ptr<RoomSession> m_room;
};