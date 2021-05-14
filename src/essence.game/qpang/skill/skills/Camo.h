#pragma once

#include <iostream>

#include "qpang/skill/Skill.h"

class Camo : public Skill
{
	uint32_t getId() override
	{
		return ItemID::SKILL_CAMO;
	}

	bool canShoot() override
	{
		std::cout << "Camo \n";

		return true;
	}
};