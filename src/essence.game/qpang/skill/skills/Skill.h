#pragma once

#include "ItemID.h"
#include "SkillTarget.h"

#include <array>
#include <memory>
#include <cstdint>

class RoomSessionPlayer;

class Skill
{
public:
	Skill() :
		m_requiredSkillPoints(0),
		m_hasDuration(false),
		m_durationInSeconds(0),
		m_isRainbowSkillCard(false),
		m_isReflectableSkillCard(false),
		m_skillTarget(SkillTarget::SELF)
	{
	}

	void bind(std::shared_ptr<RoomSessionPlayer> player)
	{
		m_player = player;
	}

	virtual uint32_t getItemId()
	{
		return ItemID::SKILL_NONE;
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

	virtual bool shouldDisableOnBeingAttacked()
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

	bool hasDuration()
	{
		return m_hasDuration;
	}

	bool isRainbowSkillCard()
	{
		return m_isRainbowSkillCard;
	}

	bool isReflectableSkillCard()
	{
		return m_isReflectableSkillCard;
	}

	uint32_t getDurationInSeconds()
	{
		return m_durationInSeconds;
	}

	uint32_t getRequiredSkillPoints()
	{
		return m_requiredSkillPoints;
	}

	SkillTarget getSkillTarget()
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
private:
};