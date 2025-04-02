#ifndef GC_PVE_MOVE_NPC_HPP
#define GC_PVE_MOVE_NPC_HPP

#include "GameNetEvent.h"

class GCPvEMoveNpc : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 npcUid;
	U16 xCoord;
	U16 zCoord;

	GCPvEMoveNpc() : GameNetEvent{ GC_PVE_MOVE_NPC, GuaranteedOrdered, DirServerToClient } {}

	GCPvEMoveNpc(const uint32_t npcUid, const uint16_t xCoord, const uint16_t zCoord)
		: GameNetEvent{ GC_PVE_MOVE_NPC, GuaranteedOrdered, DirServerToClient },
		npcUid(npcUid),
		xCoord(xCoord),
		zCoord(zCoord)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(npcUid);
		bstream->write(xCoord);
		bstream->write(zCoord);
	}

	void unpack(EventConnection* conn, BitStream* bstream) {}
	void process(EventConnection* ps) {}

	TNL_DECLARE_CLASS(GCPvEMoveNpc);
};

#endif // !GC_ARRANGED_ACCEPT_HPP