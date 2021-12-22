#ifndef GC_PVE_DOOR_HPP
#define GC_PVE_DOOR_HPP

#include "GameNetEvent.h"

class GCPvEDoor : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 objectUid; // gate uid/ gate id.
	bool flag = true; // probably a flag for opening / closing?

	GCPvEDoor() : GameNetEvent{ GC_PVE_DOOR, GuaranteedOrdered, DirServerToClient } {}
	GCPvEDoor(const uint32_t objectUid) : GameNetEvent{ GC_PVE_DOOR, GuaranteedOrdered, DirServerToClient }
	{
		this->objectUid = objectUid;
	}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(objectUid);
		bstream->writeFlag(flag);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvEDoor);
};

#endif // !GC_ARRANGED_ACCEPT_HPP