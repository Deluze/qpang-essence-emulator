#ifndef GC_PVE_DIE_NPC_HPP
#define GC_PVE_DIE_NPC_HPP

#include "GameNetEvent.h"


class GCPvEDieNpc : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 npcUid; // 88

	GCPvEDieNpc() : GameNetEvent{ GC_PVE_DIE_NPC, Guaranteed, DirServerToClient } {}
	GCPvEDieNpc(const uint32_t npcUid) : GameNetEvent{ GC_PVE_DIE_NPC, Guaranteed, DirServerToClient },
		npcUid(npcUid)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(npcUid);
	}

	void unpack(EventConnection* conn, BitStream* bstream) {}
	void process(EventConnection* ps) {}

	TNL_DECLARE_CLASS(GCPvEDieNpc);
};

#endif // !GC_ARRANGED_ACCEPT_HPP