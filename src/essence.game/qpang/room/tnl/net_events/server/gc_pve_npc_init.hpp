#ifndef GC_PVE_NPC_INIT_HPP
#define GC_PVE_NPC_INIT_HPP

#include "GameNetEvent.h"

class GCPvENpcInit final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 npcType; // 88
	U32 npcUid; // 92
	F32 xPos; // 96
	F32 yPos; // 100
	F32 zPos; // 104
	U16 rotation = 0; // 108
	U8 unk_07 = 1; // 110
	U32 unk_08 = 0; // 112

	GCPvENpcInit() : GameNetEvent{ GC_PVE_NPC_INIT, GuaranteedOrdered, DirServerToClient } {}

	GCPvENpcInit(const eNpcType npcType, const uint32_t npcUid, const Position position)
		: GameNetEvent{ GC_PVE_NPC_INIT, GuaranteedOrdered, DirServerToClient },
		npcType(static_cast<U32>(npcType)),
		npcUid(npcUid),
		xPos(position.x),
		yPos(position.y),
		zPos(position.z)
	{

	}

	GCPvENpcInit(const eNpcType npcType, const uint32_t npcUid, const Position position, U16 rotation, U8 unk_07, U32 unk_08)
		: GameNetEvent{ GC_PVE_NPC_INIT, GuaranteedOrdered, DirServerToClient },
		npcType(static_cast<U32>(npcType)),
		npcUid(npcUid),
		xPos(position.x),
		yPos(position.y),
		zPos(position.z),
		rotation(rotation),
		unk_07(unk_07),
		unk_08(unk_08)
	{

	}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(npcType);
		bstream->write(npcUid);
		bstream->write(xPos);
		bstream->write(yPos);
		bstream->write(zPos);
		bstream->write(rotation);
		bstream->write(unk_07);
		bstream->write(unk_08);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvENpcInit);
};


#endif // !GC_ARRANGED_ACCEPT_HPP