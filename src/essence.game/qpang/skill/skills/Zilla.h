#pragma once

#include <iostream>

#include "qpang/skill/Skill.h"

class 
	Zilla: public Skill
{
public:
	Zilla() = default;

	uint32_t getId() override
	{
		return ItemID::SKILL_ZILLA;
	}
};