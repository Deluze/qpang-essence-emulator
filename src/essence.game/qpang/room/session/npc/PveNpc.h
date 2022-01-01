#pragma once

#include <memory>

#include "Pathfinder.h"
#include "PveNpcData.h"
#include "RoomSessionPlayer.h"

class PveNpc
{
public:
	PveNpc() = default;
	~PveNpc() = default;

	PveNpc(PveNpcData data, const PathfinderCell& spawnCell);
	PveNpc(PveNpcWaveData data, const PathfinderCell& spawnCell);

	void tick(const std::shared_ptr<RoomSession>& roomSession);

	void handleDeath(const std::shared_ptr<RoomSession>& roomSession);

	void setLastAttackerId(uint32_t id);

	bool isNextMoveValid(Pathfinder* pathFinder, const PathfinderCell& cell) const;

	PathfinderCell getMoveCell();

	void clearPath();

	[[nodiscard]]
	bool didPathFinish() const;

	void doPathfindingMove(std::shared_ptr<RoomSession> roomSession, const PathfinderCell& cell);

	void setPosition(Pathfinder* pathFinder, const PathfinderCell& cell);

	PathfinderCell getTargetCell();

	std::shared_ptr<RoomSessionPlayer> getTargetPlayer();

	Pathfinder* getPathFinder(const std::shared_ptr<RoomSession>& roomSession) const;

	/**
	 * \brief Spawns in the npc by relaying the init event.
	 */
	void spawn(const std::shared_ptr<RoomSession>& roomSession) const;
	void spawn(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const;

	void resetPosition();

	/**
	 * \brief Resets the npc health and spawns it in at its initial position.
	 * \param roomSession The room in session.
	 */
	void respawn(const std::shared_ptr<RoomSession>& roomSession);

	/**
	 * \brief The npcs dies.
	 * \param roomSession The room in session.
	 */
	void die(const std::shared_ptr<RoomSession>& roomSession);

	/**
	 * \brief Resets the health to the base health.
	 */
	void resetHealth();

	/**
	 * \brief Subtracts the damage value from the health value.
	 * \param damage The damage to subtract from the health.
	 * \return The amount of damage taken.
	 */
	uint16_t takeDamage(uint16_t damage);

	bool getIsMovingToPlayer();

	eNpcTargetType getTargetType();

	// NOTE: Temporary attack function for an npc.
	void attack(const std::shared_ptr<RoomSession>& roomSession, Position targetPosition) const;

	bool canAttackTargetPos(Pathfinder* pathFinder);

	bool canAttackTargetPlayer(Pathfinder* pathFinder);

	void attackTargetPos(const std::shared_ptr<RoomSession>& roomSession, const Position& targetPosition);

	void attackTargetPlayer(const std::shared_ptr<RoomSession>& roomSession);

#pragma region Event handlers

	/**
	 * \brief Handles the on death event for an npc.
	 */
	void onDeath(const std::shared_ptr<RoomSession>& roomSession);

#pragma endregion

#pragma region Setters

	/**
	 * \brief Sets the uid of the npc.
	 * \param uid The uid of the npc.
	 */
	void setUid(uint32_t uid);

#pragma endregion

#pragma region Getters

	/**
	 * \brief Gets the uid for the npc.
	 * \return The uid (spawnId).
	 */
	[[nodiscard]]
	uint32_t getUid() const;

	/**
	 * \brief Gets the type of the npc.
	 * \return The type of the npc.
	 */
	[[nodiscard]]
	uint32_t getType() const;

	/**
	 * \brief Gets the initial spawn position for the npc.
	 * \return The spawn position.
	 */
	[[nodiscard]]
	Position getInitialPosition() const;

	/**
	 * \brief Gets the position for the npc.
	 * \return The position.
	 */
	[[nodiscard]]
	Position getPosition() const;

	/**
	 * \brief Gets the health for the npc.
	 * \return The current health.
	 */
	[[nodiscard]]
	uint16_t getHealth() const;

	/**
	 * \brief Gets the base health for the npc.
	 * \return The base health.
	 */
	[[nodiscard]]
	uint16_t getBaseHealth() const;

	/**
	 * \brief Gets the speed for the npc.
	 * \return The speed.
	 */
	[[nodiscard]]
	float getSpeed() const;

	/**
	 * \brief Gets the initial spawn rotation for the npc.
	 * \return The initial spawn rotation.
	 */
	[[nodiscard]]
	uint16_t getInitialRotation() const;

	/**
	 * \brief Gets the weapon item id the npc uses to attack.
	 * \return The weapon item id.
	 */
	[[nodiscard]]
	uint32_t getWeaponItemId() const;

	[[nodiscard]]
	eWeaponType getWeaponType() const;

	/**
	 * \brief Gets the spawn area id for the npc.
	 * \return The spawn area id.
	 */
	[[nodiscard]]
	uint32_t getAreaUid() const;

	/**
	 * \brief Indicates whether or not the npc should re-spawn upon death.
	 * \return true if the npc should respawn otherwise false.
	 */
	[[nodiscard]]
	bool shouldRespawn() const;

	/**
	 * \brief The body parts of the npc.
	 * \return The body parts.
	 */
	std::vector<NpcBodyPart> getBodyParts();

	/**
	 * \brief Whether or not the npc is dead.
	 * \return true if the npc is dead otherwise false.
	 */
	[[nodiscard]]
	bool isDead() const;

	/**
	 * \brief Checks if the npc possesses the given body part id.
	 * \param bodyPartId The body part to check
	 * \return true if the npc has the body part otherwise false.
	 */
	[[nodiscard]]
	bool hasBodyPart(uint32_t bodyPartId);

#pragma endregion

private:
	bool canShoot();

	void handleNoMovement(const std::shared_ptr<RoomSession>& roomSession);

	void improveTargetCell(Pathfinder* pathFinder);

	void startMovingToPos(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder, const Position& targetPos);

	void startMovingToPlayer(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder);

	void handleTargetNear(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder);

	void handleTargetNearRevenge(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder);

	void handleTargetEssencePriority(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder);

	void handleMovement(const std::shared_ptr<RoomSession>& roomSession);

	bool isPlayerValid(const std::shared_ptr<RoomSessionPlayer>& player) const;

	RoomSessionPlayer::Ptr findValidAttackerPlayer(const std::shared_ptr<RoomSession>& roomSession) const;

	RoomSessionPlayer::Ptr findClosestValidPlayer(const std::shared_ptr<RoomSession>& roomSession) const;

	/**
	 * \brief Picks a random item from the loot table and drops it.
	 * \param roomSession The current room session.
	 */
	void dropLoot(const std::shared_ptr<RoomSession>& roomSession);

	uint32_t m_lastAttackerId = 0;

	uint32_t m_type{};
	uint32_t m_uid{};

	uint32_t m_areaUid;
	uint32_t m_floorNumber;

	uint16_t m_baseHealth;
	uint16_t m_health;

	float m_speed;

	// These two are used for shooting/attacking.
	uint32_t m_weaponItemId;
	uint8_t m_weaponBodyPartId;
	eWeaponType m_weaponType;

	// How often the npcs attacks.
	uint32_t m_aiTime;
	uint32_t m_lastAttackTime = 0;

	float m_attackRange;

	// The range in width and height for the npc.
	float m_attackWidth;
	float m_attackHeight;

	// Whether or not the npc should respawn.
	bool m_shouldRespawn{};
	// The amount of time in seconds before the npcs respawns after death.
	uint32_t m_respawnTime = NULL;
	// The time at which the npc has died.
	uint64_t m_timeOfDeath = NULL;

	// Whether or not the npc can drop loot on death.
	bool m_canDropLoot;

	// Initial rotation on spawn.
	uint16_t m_initialRotation;

	Position m_initialPosition;
	Position m_position;

	Position m_staticShootingPosition;

	Position m_targetShootPosition;

	eNpcGradeType m_gradeType;
	eNpcMovementType m_movementType;
	eNpcTargetType m_targetType;

	PathfinderCell m_initialCell = {};
	PathfinderCell m_takenCell = {};
	PathfinderCell m_currentCell = {};
	PathfinderCell m_targetCell = {};

	std::vector<PathfinderCell> m_path = {};
	int m_pathIdx = 0;

	std::shared_ptr<RoomSessionPlayer> m_targetPlayer = nullptr;

	bool m_isMovingToPlayer = false;

	std::vector<NpcLootDrop> m_lootDrops{};
	std::vector<NpcBodyPart> m_bodyParts{};
};
