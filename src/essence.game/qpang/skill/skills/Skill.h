#pragma once

#include "ItemID.h"
#include "SkillTarget.h"
//#include "RoomSessionPlayer.h"

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
		m_skillTarget(SkillTarget::SELF)
	{
	}

	virtual void tick()
	{
		if (m_hasDuration && m_durationInSeconds > 0)
		{
			m_durationInSeconds--;
		}
	}
	
	void bind(std::shared_ptr<RoomSessionPlayer> player)
	{
		m_player = player;
	}

	virtual void apply()
	{

	}

	virtual uint32_t getItemId()
	{
		return ItemID::SKILL_NONE;
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

	bool hasTargetOtherThanSelf()
	{
		return (m_skillTarget != SkillTarget::SELF);
	}

	bool hasDuration()
	{
		return m_hasDuration;
	}

	uint32_t getDurationInSeconds()
	{
		return m_durationInSeconds;
	}

	uint32_t getRequiredSkillPoints()
	{
		return m_requiredSkillPoints;
	}
protected:
	SkillTarget m_skillTarget;

	bool m_hasDuration;

	uint32_t m_durationInSeconds;
	uint32_t m_requiredSkillPoints;
private:
	std::shared_ptr<RoomSessionPlayer> m_player;
};