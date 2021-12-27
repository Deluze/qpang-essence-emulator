#pragma once

#include <memory>

#include "Position.h"
#include "RoomSessionPlayer.h"

class RoomSession;

enum class eNpcGradeType : uint8_t
{
	G_EASY = 0,
	G_NORMAL = 1,
	G_HARD = 2
};

enum class eNpcMovementType : uint8_t
{
	M_NONE = 0,
	M_PATH_FINDING = 1,
	M_PATH_NODES = 2,
	M_MAX = 8
};

enum class eNpcTargetType : uint8_t
{
	T_NONE = 0,
	T_STATIC = 1,
	T_STATIC_REVENGE = 2,
	T_NEAR = 3,
	T_NEAR_REVENGE = 4,
	T_ESSENCE_PRIORITY = 5,
	T_DAMAGE = 6
};

struct NpcLootDrop
{
	uint32_t itemId;
	uint32_t probability;
};

struct NpcBodyPart
{
	uint32_t id;
	uint16_t health;
	uint32_t weaponItemId;
	uint32_t itemBoxId;
	bool isDualGun;
};

class PveNpc
{
public:
	PveNpc() = default;
	~PveNpc() = default;

	PveNpc(uint32_t type, uint16_t baseHealth, float speed, uint32_t weaponItemId, uint8_t weaponBodyPartId, uint32_t aiTime,
		float attackWidth, float attackHeight, bool shouldRespawn, uint32_t respawnTime, bool canDropLoot,
		uint16_t initialRotation, Position initialPosition, eNpcGradeType gradeType, eNpcMovementType movementType,
		eNpcTargetType targetType, std::vector<NpcLootDrop> lootDrops, std::vector<NpcBodyPart> bodyParts);

	void tick(const std::shared_ptr<RoomSession>& roomSession);

	/**
	 * \brief Spawns in the npc by relaying the init event.
	 */
	void spawn(const std::shared_ptr<RoomSession>& roomSession) const;
	void spawn(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const;

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

	// NOTE: Temporary attack function for an npc.
	void attack(const std::shared_ptr<RoomSession>& roomSession, Position targetPosition) const;

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
	/**
	 * \brief Picks a random item from the loot table and drops it.
	 * \param roomSession The current room session.
	 */
	void dropLoot(const std::shared_ptr<RoomSession>& roomSession);

	uint32_t m_type{};
	uint32_t m_uid{};

	uint16_t m_baseHealth;
	uint16_t m_health;

	float m_speed;

	// These two are used for shooting/attacking.
	uint32_t m_weaponItemId;
	uint8_t m_weaponBodyPartId;

	// How often the npcs attacks.
	uint32_t m_aiTime;

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

	eNpcGradeType m_gradeType;
	eNpcMovementType m_movementType;
	eNpcTargetType m_targetType;

	std::vector<NpcLootDrop> m_lootDrops{};
	std::vector<NpcBodyPart> m_bodyParts{};
};
