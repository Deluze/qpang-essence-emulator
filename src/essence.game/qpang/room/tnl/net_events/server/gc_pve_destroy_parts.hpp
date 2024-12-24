#ifndef GC_PVE_DESTROY_PARTS_HPP
#define GC_PVE_DESTROY_PARTS_HPP

#include "GameNetEvent.h"

class GCPvEDestroyParts final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 npcUid; // 88
	U32 bodyPartId; // 92

	GCPvEDestroyParts() : GameNetEvent{ GC_PVE_DESTROY_PARTS, GuaranteedOrdered, DirServerToClient } {}

	GCPvEDestroyParts(const uint32_t npcUid, const uint32_t bodyPartId)
		: GameNetEvent{ GC_PVE_DESTROY_PARTS, GuaranteedOrdered, DirServerToClient },
		npcUid(npcUid),
		bodyPartId(bodyPartId)
	{}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(npcUid);
		bstream->write(bodyPartId);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvEDestroyParts);
};

#endif // !GC_ARRANGED_ACCEPT_HPP