#pragma once

class GiantBeastSkill final : public Skill
{
	static constexpr uint32_t TRANSFORMATION_TIME = 9;
	static constexpr uint16_t BASE_HEALTH = 600;
	static constexpr uint32_t WEAPON_ITEM_ID = 1095368707;

public:
	GiantBeastSkill() : Skill(true, 40)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_GIANT_BEAST;
	}

	void onApply() override
	{
		Skill::onApply();

		m_player->setHealth(BASE_HEALTH, true);
		m_player->makeInvincible(TRANSFORMATION_TIME);

		m_player->getWeaponManager()->equipRainbowSkillCardWeapon(WEAPON_ITEM_ID);
	}

	void onWearOff() override
	{
		if (!m_player->isDead())
		{
			m_player->setHealth(m_player->getDefaultHealth(), true);
		}

		m_player->getWeaponManager()->unequipRainbowSkillCardWeapon();
	}

	bool isRainbowSkillCard() override
	{
		return true;
	}

	uint32_t getSkillPointCost() override
	{
		return 4;
	}
};