#pragma once

#include <iostream>
#include <cstdint>
#include <memory>
#include "qpang/ItemID.h"

class RoomSessionPlayer;

class Skill
{
public:
	Skill(bool hasDuration = false, uint16_t duration = NULL) :
		m_hasDuration(hasDuration),
		m_duration(duration)
	{

	}

	virtual ~Skill()
	{

	}

	void bind(std::shared_ptr<RoomSessionPlayer> player)
	{
		m_player = player;
	}

	virtual uint32_t getId()
	{
		return 0;
	}

	virtual void tick()
	{
		if (m_hasDuration)
			m_duration--;
	}

	virtual bool canShoot()
	{
		return true;
	}

	virtual void onShoot()
	{

	}

	virtual bool canAbility()
	{
		return true;
	}

	virtual void onDeath()
	{

	}
private:
	std::weak_ptr<RoomSessionPlayer> m_player;

	bool m_hasDuration;
	uint16_t m_duration;
};