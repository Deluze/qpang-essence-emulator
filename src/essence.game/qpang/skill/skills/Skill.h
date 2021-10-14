#pragma once

#include "ItemID.h"
#include <qpang/skill/SkillTarget.h>

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
	
	//void bind(RoomSessionPlayer::Ptr roomSessionPlayer)
	//{
	//	m_player = roomSessionPlayer;
	//}

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

	virtual bool shouldDisableOnUseOfActionCard()
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
	//RoomSessionPlayer::Ptr m_player;

	SkillTarget m_skillTarget;

	bool m_hasDuration;

	uint32_t m_durationInSeconds;
	uint32_t m_requiredSkillPoints;

private:
};