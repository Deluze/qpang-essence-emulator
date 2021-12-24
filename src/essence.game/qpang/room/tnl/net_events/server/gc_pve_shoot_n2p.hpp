#ifndef GC_PVE_SHOOT_N2P_HPP
#define GC_PVE_SHOOT_N2P_HPP

#include "GameNetEvent.h"

class GCPvEShootN2P final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 npcUid; // 88
	U32 weaponBodyPartId; // 92

	// What are the following floats for?
	F32 targetPositionX; // 96 
	F32 targetPositionY; // 100
	F32 targetPositionZ; // 104

	U64 unk_06 = 0; // 112 possible weaponId

	GCPvEShootN2P() : GameNetEvent{ GC_PVE_SHOOT_N2P, GuaranteedOrdered, DirServerToClient } {}

	GCPvEShootN2P(const uint32_t npcUid, const uint32_t weaponBodyPartId, const Position targetPosition)
		: GameNetEvent{ GC_PVE_SHOOT_N2P, GuaranteedOrdered, DirServerToClient },
		npcUid(npcUid),
		weaponBodyPartId(weaponBodyPartId),
		targetPositionX(targetPosition.x),
		targetPositionY(targetPosition.y),
		targetPositionZ(targetPosition.z)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(npcUid);
		bstream->write(weaponBodyPartId);
		bstream->write(targetPositionX);
		bstream->write(targetPositionY);
		bstream->write(targetPositionZ);
		bstream->write(unk_06);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvEShootN2P);
};

#endif // !GC_ARRANGED_ACCEPT_HPP