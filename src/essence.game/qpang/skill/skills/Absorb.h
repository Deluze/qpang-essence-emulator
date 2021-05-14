#pragma once

#include <iostream>

#include "qpang/skill/Skill.h"

class Absorb : public Skill
{
public:
	Absorb() = default;

	uint32_t getId() override
	{
		return ItemID::SKILL_ABSORB;
	}
};