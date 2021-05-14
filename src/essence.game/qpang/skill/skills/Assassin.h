#pragma once

#include <iostream>

#include "qpang/skill/Skill.h"

class Assassin : public Skill
{
public:
	Assassin() = default;

	uint32_t getId() override
	{
		return ItemID::SKILL_ASSASSIN;
	}

	bool canShoot() override
	{
		std::cout << "Assassin \n";

		return true;
	}
};