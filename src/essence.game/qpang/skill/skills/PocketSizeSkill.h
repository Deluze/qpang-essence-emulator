#pragma once

class PocketSizeSkill final : public Skill
{
public:
	PocketSizeSkill() : Skill(true, 15)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_POCKET_SIZE;
	}

	void onApply() override
	{
		Skill::onApply();

		const auto currentHealth = m_player->getHealth();
		const auto bonusHealth = static_cast<uint16_t>(currentHealth / 2);
		
		m_player->setHealth((currentHealth + bonusHealth), true);
	}

	uint32_t getSkillPointCost() override
	{
		return 2;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::SILVER;
	}
};