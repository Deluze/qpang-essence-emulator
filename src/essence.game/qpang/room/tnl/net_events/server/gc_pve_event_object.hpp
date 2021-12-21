#ifndef GC_PVE_EVENT_OBJECT_HPP
#define GC_PVE_EVENT_OBJECT_HPP

#include "GameNetEvent.h"

class GCPvEEventObject : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 objectUid;

	GCPvEEventObject() : GameNetEvent{ GC_PVE_EVENT_OBJECT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirServerToClient } {};
	GCPvEEventObject(U32 objectUid) : GameNetEvent{ GC_PVE_EVENT_OBJECT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirServerToClient }
	{
		this->objectUid = objectUid;
	}

	void pack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->write(objectUid);
	};

	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEEventObject);
};

#endif // !GC_ARRANGED_ACCEPT_HPP