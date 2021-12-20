#ifndef GC_PVE_DESTROY_OBJECT_HPP
#define GC_PVE_DESTROY_OBJECT_HPP

#include "GameNetEvent.h"

class GCPvEDestroyObject : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 objectUid;

	GCPvEDestroyObject() : GameNetEvent{ GC_PVE_DESTROY_OBJECT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirServerToClient } {};
	GCPvEDestroyObject(U32 objectUid) : GameNetEvent{ GC_PVE_DESTROY_OBJECT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirServerToClient }
	{
		this->objectUid = objectUid;
	}
	
	void pack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->write(objectUid);
	};

	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEDestroyObject);
};

#endif // !GC_ARRANGED_ACCEPT_HPP