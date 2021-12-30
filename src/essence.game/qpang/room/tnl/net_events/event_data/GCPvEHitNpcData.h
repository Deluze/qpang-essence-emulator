#pragma once

#include <memory>

#include "eHitLocation.h"
#include "eWeaponType.h"
#include "Position.h"

struct GCPvEHitNpcData
{
	uint32_t playerId;
	uint32_t targetNpcUid;
	uint32_t unk_03;
	Position impactPos;
	Position impactPosOffset;
	uint32_t entityId;
	uint8_t unk_11;
	uint8_t bodyPartId;
	uint32_t weaponItemId;
	uint64_t weaponCardId;
	eWeaponType weaponType;
	eHitLocation hitLocation;
	uint8_t unk_17;
	uint32_t unk_18;
	uint16_t damageDealt;
	uint8_t killStreak;
	uint32_t unk_21;
};
