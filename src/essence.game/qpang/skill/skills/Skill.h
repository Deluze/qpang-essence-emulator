#pragma once

#include <cstdint>
#include <memory>

#include <ItemId.h>
#include "SkillRateType.h"
#include "SkillTargetType.h"

class RoomSessionPlayer;

class Skill  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	inline static constexpr uint8_t GLOBAL_MAX_USE_COUNT = 3;

	explicit Skill(const bool hasDuration = false, const uint32_t duration = 0) :
		m_hasDuration(hasDuration),
		m_duration(duration),
		m_initialDuration(duration),
		m_useCount(0),
		m_maxUseCount(1)
	{
	}

	virtual ~Skill() = default;

	void bind(const std::shared_ptr<RoomSessionPlayer>& player)
	{
		m_player = player;
	}

	void use()
	{
		m_useCount++;
	}

	void setMaxUseCount(const uint16_t count)
	{
		m_maxUseCount = count;
	}

	uint16_t canUse() const
	{
		return (m_useCount < GLOBAL_MAX_USE_COUNT) && (m_useCount < m_maxUseCount);
	}

	uint16_t getUseCount() const
	{
		return m_useCount;
	}

	[[nodiscard]] bool hasDuration() const
	{
		return m_hasDuration;
	}

	[[nodiscard]] uint32_t getDuration() const
	{
		return m_duration;
	}

	virtual uint32_t getItemId()
	{
		return SKILL_NONE;
	}

	virtual void tick()
	{
		if (m_hasDuration && m_duration > 0)
		{
			m_duration--;
		}
	}

	virtual void onApply()
	{
	}

	virtual void onWearOff()
	{
		m_duration = m_initialDuration;
	}

	virtual bool isReflectableSkillCard()
	{
		return false;
	}

	virtual uint32_t getSkillPointCost()
	{
		return 0;
	}

	virtual SkillTargetType getSkillTargetType()
	{
		return SkillTargetType::SELF;
	}

	virtual SkillRateType getSkillRateType()
	{
		return SkillRateType::BRONZE;
	}

#pragma region Disable SkillCard Handlers
	// Invisible Skill
	virtual bool shouldDisableWhenDamageIsTaken()
	{
		return false;
	}

	// Invisible Skill
	virtual bool shouldDisableWhenGameItemIsPickedUp()
	{
		return false;
	}

	// Invisible Skill
	virtual bool shouldDisableWhenPerformingRollAction()
	{
		return false;
	}
#pragma endregion Disable SkillCard

	// Replay Skill
	virtual bool isReplaySkill()
	{
		return false;
	}

	// Reflect Skill
	virtual bool isReflectSkill()
	{
		return false;
	}

	// Assassin Skill
	virtual bool isAssassinSkill()
	{
		return false;
	}

	// Mental Up Skill
	virtual bool isMentalUpSkill()
	{
		return false;
	}

	// Stunt Up Skill
	virtual bool isStuntUpSkill()
	{
		return false;
	}

	// Iron Wall Skill
	virtual bool IsIronWallSkill()
	{
		return false;
	}

	// Melee Only Skill
	virtual bool shouldOnlyTakeMeleeDamage()
	{
		return false;
	}

	// Melee Only Skill
	virtual bool shouldOnlyDealMeleeDamage()
	{
		return false;
	}

	// EnergyShield Skill
	virtual bool isEnergyShieldSkill()
	{
		return false;
	}
protected:
	bool m_hasDuration;

	uint32_t m_duration;
	uint32_t m_initialDuration;

	uint16_t m_useCount;
	uint16_t m_maxUseCount;

	std::shared_ptr<RoomSessionPlayer> m_player;
};
