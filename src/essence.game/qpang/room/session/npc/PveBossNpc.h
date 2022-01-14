#pragma once

#include <memory>

#include "PveNpc.h"

class PveBossNpc final : public PveNpc
{
public:
	PveBossNpc() = default;
	~PveBossNpc() override = default;

	PveBossNpc(PveNpcData data, const PathfinderCell& spawnCell);

	void tick(const std::shared_ptr<RoomSession>& roomSession) override;

	float calculateHitDamage(const CGPvEHitNpcData& data) override;

	void spawn(const std::shared_ptr<RoomSession>& roomSession) const override;
	void spawn(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const override;

	void attackTargetPlayer(const std::shared_ptr<RoomSession>& roomSession) override;

	void startMovingToPlayer(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder) override;

	void onDeath(const std::shared_ptr<RoomSession>& roomSession) override;

	/**
	 * \brief Indicates whether or not the boss can start its special attack.
	 * \return true if the boss can start its special attack, otherwise false.
	 */
	[[nodiscard]]
	bool canStartSpecialAttack() const;

private:
	void handleSpecialAttackEnd(const std::shared_ptr<RoomSession>& roomSession);
	void handlePerformSpecialAttackShoot(const std::shared_ptr<RoomSession>& roomSession);

	/**
	 * \brief Starts the special attack.
	 * \param roomSession The room in session.
	 */
	void startSpecialAttack(const std::shared_ptr<RoomSession>& roomSession);

	/**
	 * \brief Ends the special attack.
	 * \param roomSession The room in session.
	 */
	void endSpecialAttack(const std::shared_ptr<RoomSession>& roomSession);

	/**
	 * \brief Performs the special attack shooting.
	 * \param roomSession The room in session.
	 */
	void performSpecialAttackShoot(const std::shared_ptr<RoomSession>& roomSession) const;

	bool m_isInSpecialAttack = false;
	uint16_t m_specialAttackDamageTreshold{};

	uint16_t m_previousSpecialAttackHealth{};

	uint32_t m_specialAttackDuration{};
	uint32_t m_lastSpecialAttackTime = NULL;
};
