#pragma once

class TranseSkill final : public Skill
{
	static constexpr uint32_t TRANSFORMATION_TIME = 6;
	static constexpr uint16_t BASE_HEALTH = 500;
	static constexpr uint32_t WEAPON_ITEM_ID = 1095303170;

public:
	TranseSkill() : Skill(true, 40)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_TRANSE;
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
		Skill::onWearOff();

		if (!m_player->isDead())
		{
			m_player->setHealth(m_player->getDefaultHealth(), true);
		}

		m_player->getWeaponManager()->unequipRainbowSkillCardWeapon();
	}

	uint32_t getSkillPointCost() override
	{
		return 4;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::RAINBOW;
	}
};