#pragma once

#include <cstdint>
#include <unordered_map>

#include <qpang/room/game_mode/GameMode.h>
#include <qpang/skill/skills/Skill.h>

#include <memory>
#include <unordered_map>
#include <functional>

class SkillManager
{
public:
	SkillManager();

	void initialize();

	std::vector<std::function<std::unique_ptr<Skill>()>> SkillManager::getSkillsForGameMode(uint8_t mode);
private:
	void initializeDeathMatchSkills();
	void initializeTeamDeathMatchSkills();

	std::unordered_map<uint8_t, std::vector<std::function<std::unique_ptr<Skill>()>>> m_skills;
};