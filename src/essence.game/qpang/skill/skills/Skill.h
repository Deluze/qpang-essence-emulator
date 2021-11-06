#pragma once

#include <cstdint>
#include <memory>

#include "ItemId.h"

#include "SkillRateType.h"
#include "SkillTargetType.h"

class RoomSessionPlayer;

class Skill  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	explicit Skill(const bool hasDuration = false, const uint32_t duration = 0) :
		m_hasDuration(hasDuration),
		m_duration(duration),
		m_initialDuration(duration),
		m_usesLeftCount(1)
	{
	}

	virtual ~Skill() = default;

	void bind(const std::shared_ptr<RoomSessionPlayer>& player)
	{
		m_player = player;
	}

	void use()
	{
		if (m_usesLeftCount > 0)
		{
			m_usesLeftCount -= 1;
		}
	}

	void setUsesLeftCount(const uint32_t usesLeftCount)
	{
		m_usesLeftCount = usesLeftCount;
	}

	bool hasUsesLeft() const
	{
		return (m_usesLeftCount > 0);
	}

	uint32_t getUsesLeftCount() const
	{
		return m_usesLeftCount;
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
#pragma endregion Disable SkillCard

	virtual bool shouldInstantlyRespawnOnDeath()
	{
		return false;
	}

	// TODO: Refactor (rename).
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

	virtual bool shouldReceiveReducedDamageFromAllSources()
	{
		return false;
	}

	virtual bool shouldIgnoreDamageFromAllSources()
	{
		return false;
	}
protected:
	bool m_hasDuration;

	uint32_t m_duration;
	uint32_t m_initialDuration;

	uint32_t m_usesLeftCount;

	std::shared_ptr<RoomSessionPlayer> m_player;
};
