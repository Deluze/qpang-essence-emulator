#ifndef GC_PVE_HIT_N2P_HPP
#define GC_PVE_HIT_N2P_HPP

#include "GameNetEvent.h"

class GCPvEHitN2P final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 npcUid; // 88
	U32 npcBodyPartId; // 92
	F32 impactPosX; // 96
	F32 impactPosY; // 100
	F32 impactPosZ; // 104
	F32 impactOffsetPosX; // 108
	F32 impactOffsetPosY; // 112
	F32 impactOffsetPosZ; // 116

	U8 unk_09 = 0; // 120
	U8 bodyPartHitLocation = 0; // 121

	// One of these unknowns must be damageDealt or healthLeft (I think).
	U32 unk_11 = 0; // 124
	U32 unk_12 = 0; // 128

	GCPvEHitN2P() : GameNetEvent{ GC_PVE_HIT_N2P, GuaranteedOrdered, DirServerToClient } {}
	GCPvEHitN2P(const uint32_t npcUid, const uint32_t npcBodyPartId, const Position impactPosition, const Position impactOffsetPosition, 
		const uint8_t unk09,const uint8_t bodyPartHitLocation, const uint32_t unk11, const uint32_t unk12)
		: GameNetEvent{ GC_PVE_HIT_N2P, GuaranteedOrdered, DirServerToClient },
		npcUid(npcUid),
		npcBodyPartId(npcBodyPartId),
		impactPosX(impactPosition.x),
		impactPosY(impactPosition.y),
		impactPosZ(impactPosition.z),
		impactOffsetPosX(impactOffsetPosition.x),
		impactOffsetPosY(impactOffsetPosition.y),
		impactOffsetPosZ(impactOffsetPosition.z),
		unk_09(unk09),
		bodyPartHitLocation(bodyPartHitLocation),
		unk_11(unk11),
		unk_12(unk12)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(npcUid);
		bstream->write(npcBodyPartId);
		bstream->write(impactPosX);
		bstream->write(impactPosY);
		bstream->write(impactPosZ);
		bstream->write(impactOffsetPosX);
		bstream->write(impactOffsetPosY);
		bstream->write(impactOffsetPosZ);
		bstream->write(unk_09);
		bstream->write(bodyPartHitLocation);
		bstream->write(unk_11);
		bstream->write(unk_12);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvEHitN2P);
};

#endif // !GC_ARRANGED_ACCEPT_HPP