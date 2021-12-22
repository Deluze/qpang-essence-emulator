#ifndef GC_PVE_DOOR_HPP
#define GC_PVE_DOOR_HPP

#include "GameNetEvent.h"

class GCPvEDoor : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 objectUid; // gate uid/ gate id.
	bool shouldOpen; // probably a flag for opening / closing?

	GCPvEDoor() : GameNetEvent{ GC_PVE_DOOR, GuaranteedOrdered, DirServerToClient } {}
	GCPvEDoor(const uint32_t objectUid, const bool shouldOpen)
		: GameNetEvent{ GC_PVE_DOOR, GuaranteedOrdered, DirServerToClient },
		objectUid(objectUid),
		shouldOpen(shouldOpen)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(objectUid);
		bstream->writeFlag(shouldOpen);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvEDoor);
};

#endif // !GC_ARRANGED_ACCEPT_HPP