#ifndef GC_PVE_SPECIAL_ATTACK_HPP
#define GC_PVE_SPECIAL_ATTACK_HPP


#include "GameNetEvent.h"

class GCPvESpecialAttack final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 npcUid = 0;
	U32 unk_02 = 50;
	U8 unk_03 = 0;

	GCPvESpecialAttack() : GameNetEvent{ GC_PVE_SPECIAL_ATTACK, GuaranteedOrdered, DirServerToClient } {}
	GCPvESpecialAttack(const uint32_t npcUid, const uint32_t unk02, const uint8_t unk03)
		: GameNetEvent{ GC_PVE_SPECIAL_ATTACK, GuaranteedOrdered, DirServerToClient },
		npcUid(npcUid), unk_02(unk02), unk_03(unk03)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream) override
	{

	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvESpecialAttack);
};

#endif // !GC_ARRANGED_ACCEPT_HPP