#ifndef GC_PVE_HIT_NPC_HPP
#define GC_PVE_HIT_NPC_HPP

#include "GameNetEvent.h"
#include "GCPvEHitNpcData.h"

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
	U32 entityId; // 92
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

	GCPvEHitNpc(GCPvEHitNpcData data) : GameNetEvent{ GC_PVE_HIT_NPC, GuaranteedOrdered, DirServerToClient },
		playerId(data.playerId),
		targetNpcUid(data.targetNpcUid),
		unk_03(data.unk_03),
		impactPosX(data.impactPos.x),
		impactPosY(data.impactPos.y),
		impactPosZ(data.impactPos.z),
		impactOffsetPosX(data.impactPosOffset.x),
		impactOffsetPosY(data.impactPosOffset.y),
		impactOffsetPosZ(data.impactPosOffset.z),
		entityId(data.entityId),
		unk_11(data.unk_11),
		bodyPartId(data.bodyPartId),
		weaponItemId(data.weaponItemId),
		weaponCardId(data.weaponCardId),
		weaponType(static_cast<uint8_t>(data.weaponType)),
		hitLocation(static_cast<uint8_t>(data.hitLocation)),
		unk_17(data.unk_17),
		unk_18(data.unk_18),
		damageDealt(data.damageDealt),
		hasTargetDied(static_cast<uint8_t>(data.hasTargetDied)),
		unk_21(data.unk_21)
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
		bstream->write(entityId);
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