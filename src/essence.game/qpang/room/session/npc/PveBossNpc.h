#pragma once

#include <memory>

#include "PveNpc.h"

class PveBossNpc : public PveNpc
{
public:
	PveBossNpc() = default;
	~PveBossNpc() = default;

	PveBossNpc(PveNpcData data, const PathfinderCell& spawnCell);

	void tick(const std::shared_ptr<RoomSession>& roomSession) override;

	float calculateHitDamage(const class CGPvEHitNpcData& data) override;

	virtual void spawn(const std::shared_ptr<RoomSession>& roomSession) const override;
	virtual void spawn(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const override;
private:
	bool m_isInSpecialAttack;
	float m_specialAttackDamageTreshold;
};
