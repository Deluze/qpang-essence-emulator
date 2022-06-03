#ifndef GC_PVE_DESTROY_OBJECT_HPP
#define GC_PVE_DESTROY_OBJECT_HPP

#include "GameNetEvent.h"

class GCPvEDestroyObject final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 objectUid;

	GCPvEDestroyObject() : GameNetEvent{ GC_PVE_DESTROY_OBJECT, GuaranteedOrdered, DirServerToClient } {}

	explicit GCPvEDestroyObject(const U32 objectUid)
		: GameNetEvent{ GC_PVE_DESTROY_OBJECT, GuaranteedOrdered, DirServerToClient },
		objectUid(objectUid)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(objectUid);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvEDestroyObject);
};

#endif // !GC_ARRANGED_ACCEPT_HPP