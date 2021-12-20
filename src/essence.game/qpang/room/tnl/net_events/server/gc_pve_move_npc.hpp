#ifndef GC_PVE_MOVE_NPC_HPP
#define GC_PVE_MOVE_NPC_HPP

#include "GameNetEvent.h"

class GCPvEMoveNpc : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 npcUid;
	U16 unk_02;
	U16 unk_03;

	GCPvEMoveNpc() : GameNetEvent{ GC_PVE_MOVE_NPC, GuaranteedOrdered, DirServerToClient } {}

	GCPvEMoveNpc(const uint32_t npcUid, const uint16_t unk_02, const uint16_t unk_03)
		: GameNetEvent{ GC_PVE_MOVE_NPC, GuaranteedOrdered, DirServerToClient },
		npcUid(npcUid),
		unk_02(unk_02),
		unk_03(unk_03)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(npcUid);

		bstream->write(unk_02);
		bstream->write(unk_03);
	}

	void unpack(EventConnection* conn, BitStream* bstream) {}
	void process(EventConnection* ps) {}

	TNL_DECLARE_CLASS(GCPvEMoveNpc);
};

#endif // !GC_ARRANGED_ACCEPT_HPP