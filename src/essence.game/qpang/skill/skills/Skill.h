#pragma once

#include <cstdint>
#include <memory>

#include "ItemID.h"
#include "SkillTarget.h"

class RoomSessionPlayer;

class Skill
{
public:
	Skill() :
		m_skillTarget(SkillTarget::SELF),
		m_hasDuration(false),
		m_durationInSeconds(0),
		m_requiredSkillPoints(0),
		m_isReflectableSkillCard(false),
		m_isRainbowSkillCard(false)
	{
	}

	void bind(const std::shared_ptr<RoomSessionPlayer> player)
	{
		m_player = player;
	}

	virtual uint32_t getItemId()
	{
		return SKILL_NONE;
	}

	virtual void tick()
	{
		if (m_hasDuration && m_durationInSeconds > 0)
		{
			m_durationInSeconds--;
		}
	}

	virtual void onApply()
	{
	}

	virtual void onWearOff()
	{
	}

	virtual bool shouldDisableOnDamageReceive()
	{
		return false;
	}

	virtual bool shouldDisableOnGameItemPickup()
	{
		return false;
	}

	virtual bool shouldDisableOnRollAction()
	{
		return false;
	}

	virtual bool shouldReflectSkillCard()
	{
		return false;
	}

	virtual bool shouldInstantlyKillEnemyWithMeleeWeapon()
	{
		return false;
	}

	virtual bool shouldReceiveReducedDamageFromLaunchers()
	{
		return false;
	}

	[[nodiscard]] bool hasDuration() const
	{
		return m_hasDuration;
	}

	[[nodiscard]] bool isRainbowSkillCard() const
	{
		return m_isRainbowSkillCard;
	}

	[[nodiscard]] bool isReflectableSkillCard() const
	{
		return m_isReflectableSkillCard;
	}

	[[nodiscard]] uint32_t getDurationInSeconds() const
	{
		return m_durationInSeconds;
	}

	[[nodiscard]] uint32_t getRequiredSkillPoints() const
	{
		return m_requiredSkillPoints;
	}

	[[nodiscard]] SkillTarget getSkillTarget() const
	{
		return m_skillTarget;
	}
protected:
	std::shared_ptr<RoomSessionPlayer> m_player;

	SkillTarget m_skillTarget;

	bool m_hasDuration;

	uint32_t m_durationInSeconds;
	uint32_t m_requiredSkillPoints;

	bool m_isReflectableSkillCard;
	bool m_isRainbowSkillCard;
};
