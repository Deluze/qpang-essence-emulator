#ifndef GC_PVE_HIT_NPC_TO_OBJECT_HPP
#define GC_PVE_HIT_NPC_TO_OBJECT_HPP

#include "GameNetEvent.h"

class GCPvEHitNpcToObject final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 objectUid; // 88
	U16 remainingHealth; // 92
	U16 damageDealt; // 94

	GCPvEHitNpcToObject() : GameNetEvent{ GC_PVE_HIT_NPC_TO_OBJECT, GuaranteedOrdered,DirServerToClient } {}

	GCPvEHitNpcToObject(const uint32_t objectUid, const uint16_t remainingHealth, const uint16_t damageDealt)
		: GameNetEvent{ GC_PVE_HIT_NPC_TO_OBJECT, GuaranteedOrdered,DirServerToClient },
		objectUid(objectUid),
		remainingHealth(remainingHealth),
		damageDealt(damageDealt)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(objectUid);
		bstream->write(remainingHealth);
		bstream->write(damageDealt);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvEHitNpcToObject);
};


#endif // !GC_ARRANGED_ACCEPT_HPP