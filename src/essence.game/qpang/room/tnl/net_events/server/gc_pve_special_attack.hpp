#ifndef GC_PVE_SPECIAL_ATTACK_HPP
#define GC_PVE_SPECIAL_ATTACK_HPP


#include "GameNetEvent.h"

class GCPvESpecialAttack final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 npcUid = 0; // 88
	U32 unk_02 = 0; // 92
	U8 resetAttack = 0; // 96 1: npc goes to previous pos, 0: npc goes to y 20

	GCPvESpecialAttack() : GameNetEvent{ GC_PVE_SPECIAL_ATTACK, GuaranteedOrdered, DirServerToClient } {}
	GCPvESpecialAttack(const uint32_t npcUid, const uint32_t unk02, const uint8_t resetAttack)
		: GameNetEvent{ GC_PVE_SPECIAL_ATTACK, GuaranteedOrdered, DirServerToClient },
		npcUid(npcUid),
		unk_02(unk02),
		resetAttack(resetAttack)
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