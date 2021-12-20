#ifndef GC_PVE_SHOOT_N2P_HPP
#define GC_PVE_SHOOT_N2P_HPP

#include "GameNetEvent.h"

class GCPvEShootN2P : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 npcUid; // 88
	U32 playerId; // 92 possible playerId?
	// What are the following floats for?
	F32 xPos; // 96 
	F32 yPos; // 100
	F32 zPos; // 104
	U64 unk_06 = 1095368724; // 112 possible weaponId, can not be 0 or crash.

	GCPvEShootN2P() : GameNetEvent{ GC_PVE_SHOOT_N2P, GuaranteedOrdered, DirServerToClient } {}

	GCPvEShootN2P(const uint32_t npcUid, const uint32_t playerId, const Position position)
		: GameNetEvent{ GC_PVE_SHOOT_N2P, GuaranteedOrdered, DirServerToClient },
		npcUid(npcUid),
		playerId(playerId),
		xPos(position.x),
		yPos(position.y),
		zPos(position.z)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(npcUid);
		bstream->write(playerId);
		bstream->write(xPos);
		bstream->write(yPos);
		bstream->write(zPos);
		bstream->write(unk_06);
	}

	void unpack(EventConnection* conn, BitStream* bstream) {}
	void process(EventConnection* ps) {}

	TNL_DECLARE_CLASS(GCPvEShootN2P);
};

#endif // !GC_ARRANGED_ACCEPT_HPP