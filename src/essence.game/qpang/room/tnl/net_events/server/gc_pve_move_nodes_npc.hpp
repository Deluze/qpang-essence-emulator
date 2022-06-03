#ifndef GC_PVE_MOVE_NODES_NPC_HPP
#define GC_PVE_MOVE_NODES_NPC_HPP

#include "GameNetEvent.h"

class GCPvEMoveNodesNpc : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 npcUid;
	U32 nodeCount;

	GCPvEMoveNodesNpc() : GameNetEvent{ GC_PVE_MOVE_NODES_NPC, GuaranteedOrdered, DirAny } {}

	// TODO: Add vector of nodes to constructor to be used in the pack method.
	GCPvEMoveNodesNpc(const uint32_t npcUid) : GameNetEvent{ GC_PVE_MOVE_NODES_NPC, GuaranteedOrdered, DirAny },
		npcUid(npcUid)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(npcUid);
		bstream->write(nodeCount);

		for (uint32_t i = 0; i < nodeCount; i++)
		{
			bstream->write(static_cast<U16>(0));
			bstream->write(static_cast<U16>(0));
		}
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvEMoveNodesNpc);
};

#endif // !GC_ARRANGED_ACCEPT_HPP