#ifndef GC_PVE_HIT_NPC_HPP
#define GC_PVE_HIT_NPC_HPP

#include "GameNetEvent.h"

class GCPvEHitNpc : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 playerId; // 140
	U32 targetNpcUid; // 144
	U32 unk_03; // 88
	F32 impactPosX; // 116
	F32 impactPosY; // 120
	F32 impactPosZ; // 124
	F32 impactOffsetPosX; // 128
	F32 impactOffsetPosY; // 132
	F32 impactOffsetPosZ; // 136
	U32 unk_10; // 92
	U8 unk_11; // 148
	U8 bodyPartId; // 149
	U32 weaponItemId; // 96
	U64 weaponCardId; // 108 104
	U8 weaponType; // 112
	U8 hitLocation; // 150
	U8 unk_17; // 151
	U32 unk_18; // 152 
	U16 damageDealt; // 156
	U8 hasTargetDied; // 158
	U32 unk_21; // 160 // ?

	GCPvEHitNpc() : GameNetEvent{ GC_PVE_HIT_NPC, GuaranteedOrdered, DirServerToClient } {}

	GCPvEHitNpc(
		const uint32_t playerId, const uint32_t targetNpcUid, const uint32_t unk_03,
		const float impactPosX, const float impactPosY, const float impactPosZ, const float impactOffsetPosX, const float impactOffsetPosY, const float impactOffsetPosZ,
		const uint32_t unk_10, const uint8_t unk_11, const uint8_t bodyPartId, const uint32_t weaponItemId, const uint64_t weaponCardId, const uint8_t weaponType, const uint8_t hitLocation,
		const uint8_t unk_17, const uint32_t unk_18, const uint16_t damageDealt, const uint8_t hasTargetDied, const uint32_t unk_21)
		: GameNetEvent{ GC_PVE_HIT_NPC, GuaranteedOrdered, DirServerToClient },
		playerId(playerId),
		targetNpcUid(targetNpcUid),
		unk_03(unk_03),
		impactPosX(impactPosX),
		impactPosY(impactPosY),
		impactPosZ(impactPosZ),
		impactOffsetPosX(impactOffsetPosX),
		impactOffsetPosY(impactOffsetPosY),
		impactOffsetPosZ(impactOffsetPosZ),
		unk_10(unk_10),
		unk_11(unk_11),
		bodyPartId(bodyPartId),
		weaponItemId(weaponItemId),
		weaponCardId(weaponCardId),
		weaponType(weaponType),
		hitLocation(hitLocation),
		unk_17(unk_17),
		unk_18(unk_18),
		damageDealt(damageDealt),
		hasTargetDied(hasTargetDied),
		unk_21(unk_21)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(playerId);
		bstream->write(targetNpcUid);
		bstream->write(unk_03);
		bstream->write(impactPosX);
		bstream->write(impactPosY);
		bstream->write(impactPosZ);
		bstream->write(impactOffsetPosX);
		bstream->write(impactOffsetPosY);
		bstream->write(impactOffsetPosZ);
		bstream->write(unk_10);
		bstream->write(unk_11);
		bstream->write(bodyPartId);
		bstream->write(weaponItemId);
		bstream->write(weaponCardId);
		bstream->write(weaponType);
		bstream->write(hitLocation);
		bstream->write(unk_17);
		bstream->write(unk_18);
		bstream->write(damageDealt);
		bstream->write(hasTargetDied);
		bstream->write(unk_21);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvEHitNpc);
};


#endif // !GC_ARRANGED_ACCEPT_HPP