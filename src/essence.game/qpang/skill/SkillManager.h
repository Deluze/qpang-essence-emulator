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

	std::unordered_map<uint32_t, std::function<std::unique_ptr<Skill>()>> getSkillsForGameMode(uint8_t mode);
private:
	std::unordered_map<uint8_t, std::unordered_map<uint32_t, std::function<std::unique_ptr<Skill>()>>> m_skills;
};