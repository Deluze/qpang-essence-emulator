#pragma once

class WeaponSealSkill final : public Skill
{
public:
	WeaponSealSkill() : Skill(true, 10)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_WEAPON_SEAL;
	}

	uint32_t getSkillPointCost() override
	{
		return 2;
	}

	SkillTargetType getSkillTarget() override
	{
		return SkillTargetType::ENEMY;
	}

	bool isReflectableSkillCard() override
	{
		return true;
	}
};