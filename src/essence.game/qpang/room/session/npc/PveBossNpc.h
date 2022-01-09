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

	float calculateHitDamage(const CGPvEHitNpcData& data) override;

	void spawn(const std::shared_ptr<RoomSession>& roomSession) const override;
	void spawn(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const override;

	/**
	 * \brief Indicates whether or not the boss can start its special attack.
	 * \return true if the boss can start its special attack, otherwise false.
	 */
	[[nodiscard]]
	bool canStartSpecialAttack() const;

	/**
	 * \brief Starts the special attack.
	 * \param roomSession The room in session.
	 */
	void startSpecialAttack(const std::shared_ptr<RoomSession>& roomSession);

	void endSpecialAttack(const std::shared_ptr<RoomSession>& roomSession);

private:
	bool m_isInSpecialAttack;
	uint16_t m_specialAttackDamageTreshold;

	uint16_t m_previousSpecialAttackHealth;

	uint32_t m_specialAttackDuration = 30;
	uint32_t m_lastSpecialAttackTime = NULL;
};
