#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

class Skill;

class SkillManager
{
public:
	SkillManager();

	void initialize();

	std::unordered_map<uint32_t, std::function<std::unique_ptr<Skill>()>> getSkillsForMode(uint8_t mode);
private:
	std::unordered_map<uint8_t, std::unordered_map<uint32_t, std::function<std::unique_ptr<Skill>()>>> m_skills;
};