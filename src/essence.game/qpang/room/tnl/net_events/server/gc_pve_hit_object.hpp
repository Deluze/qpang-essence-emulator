#ifndef GC_PVE_HIT_OBJECT_HPP
#define GC_PVE_HIT_OBJECT_HPP

#include "GameNetEvent.h"

class GCPvEHitObject final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 playerId; // 88
	U32 objectUid; // 92
	U32 weaponItemId; // 96
	U8 cmd; // 100
	U16 unk_05; // 102 // 0 = object dissapears so maybe it's like health left or something.. ?
	U16 damageDealt; // 104

	GCPvEHitObject() : GameNetEvent{ GC_PVE_HIT_OBJECT, GuaranteedOrdered, DirServerToClient } {}

	GCPvEHitObject(const uint32_t playerId, const uint32_t objectUid, const uint32_t weaponItemId, const uint8_t cmd, const uint16_t damageDealt)
		: GameNetEvent{ GC_PVE_HIT_OBJECT, GuaranteedOrdered, DirServerToClient },
		playerId(playerId),
		objectUid(objectUid),
		weaponItemId(weaponItemId),
		cmd(cmd),
		damageDealt(damageDealt)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(playerId);
		bstream->write(objectUid);
		bstream->write(weaponItemId);
		bstream->write(cmd);
		bstream->write(unk_05);
		bstream->write(damageDealt);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvEHitObject);
};

#endif // !GC_ARRANGED_ACCEPT_HPP