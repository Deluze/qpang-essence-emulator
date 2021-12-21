#ifndef GC_PVE_EVENT_OBJECT_HPP
#define GC_PVE_EVENT_OBJECT_HPP

#include "GameNetEvent.h"

class GCPvEEventObject : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 objectUid;
	bool flag = true;

	GCPvEEventObject() : GameNetEvent{ GC_PVE_EVENT_OBJECT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirServerToClient } {};
	GCPvEEventObject(U32 objectUid, bool flag) : GameNetEvent{ GC_PVE_EVENT_OBJECT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirServerToClient }
	{
		this->objectUid = objectUid;
		this->flag = flag;
	}

	void pack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->write(objectUid);
		bstream->writeFlag(flag);
	};

	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEEventObject);
};

#endif // !GC_ARRANGED_ACCEPT_HPP