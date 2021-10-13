#pragma once

#include <cstdint>
#include <unordered_map>

#include <qpang/room/game_mode/GameMode.h>
#include <qpang/skill/skills/Skill.h>

class SkillManager
{
public:
	enum SkillID : uint32_t
	{
		SHOT_MOVE = 1258356738
	};

	SkillManager();

	void initialize();

	std::unordered_map<uint32_t, std::shared_ptr<Skill>> getSkillsForGameMode(uint8_t mode);
private:
	std::unordered_map<uint8_t, std::unordered_map<uint32_t, std::shared_ptr<Skill>>> m_skills;
};