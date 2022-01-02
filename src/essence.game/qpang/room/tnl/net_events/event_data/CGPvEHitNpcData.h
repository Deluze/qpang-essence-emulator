#pragma once

#include "eHitLocation.h"
#include "eWeaponType.h"
#include "Position.h"
#include "PveNpc.h"
#include "RoomSessionPlayer.h"

struct CGPvEHitNpcData
{
	std::shared_ptr<RoomSessionPlayer> roomSessionPlayer;
	std::shared_ptr<PveNpc> targetNpc;
	uint32_t unk_03;
	Position impactPos;
	Position impactPosOffset;
	uint32_t entityId;
	uint8_t unk_11;
	NpcBodyPart bodyPart;
	Weapon weaponUsed;
	uint64_t weaponCardId;
	eWeaponType weaponType;
	eHitLocation hitLocation;
	uint8_t unk_18;
	uint32_t unk_19;
};
