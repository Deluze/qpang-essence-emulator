#pragma once

#include "eWeaponType.h"
#include "Position.h"

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

enum class eNpcBodyPartAttributeType : uint8_t
{
	A_DEFAULT = 0,
	A_INCAPACITY = 1,
	A_DESTROY = 2
};

enum class eNpcBodyPartDamageType : uint8_t
{
	D_DEFAULT = 0,
	D_EXCEPT_SPLASH = 1,
	D_ONLY_NEARWEAPON = 2
};

enum class eNpcBodyPartPartType : uint8_t
{
	T_DEFAULT = 0,
	T_ATTACK = 1,
	T_DEFENSE = 2
};

struct NpcBodyPart
{
	uint32_t id;
	uint16_t health;
	uint32_t weaponItemId;
	uint32_t itemBoxId;
	bool isDualGun;
	eNpcBodyPartAttributeType attributeType;
	eNpcBodyPartDamageType damageType;
	eNpcBodyPartPartType partType;
};

struct PveNpcData
{
	uint32_t uid;
	uint32_t areaUid;
	uint32_t floorNumber;
	uint32_t type;
	uint16_t baseHealth;
	float speed;
	uint32_t weaponItemId;
	uint8_t weaponBodyPartId;
	eWeaponType weaponType;
	uint32_t aiTime;
	float attackRange;
	float attackWidth;
	float attackHeight;
	bool shouldRespawn;
	uint32_t respawnTime;
	bool canDropLoot;
	uint16_t initialRotation;
	Position initialPosition;
	Position staticShootingPosition;
	eNpcGradeType gradeType;
	eNpcMovementType movementType;
	eNpcTargetType targetType;
	std::vector<NpcLootDrop> lootDrops;
	std::vector<NpcBodyPart> bodyParts;
};

struct PveNpcWaveData
{
	uint32_t type;
	uint16_t baseHealth;
	float speed;
	uint32_t weaponItemId;
	uint8_t weaponBodyPartId;
	eWeaponType weaponType;
	uint32_t aiTime;
	float attackRange;
	float attackWidth;
	float attackHeight;
	bool canDropLoot;
	Position initialPosition;
	eNpcGradeType gradeType;
	eNpcMovementType movementType;
	eNpcTargetType targetType;
	std::vector<NpcLootDrop> lootDrops;
	std::vector<NpcBodyPart> bodyParts;
};
